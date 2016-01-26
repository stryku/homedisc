#pragma once

#include "FilesystemEntry.hpp"
#include "FilesystemEntryDifference.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <unordered_set>
#include <sstream>
#include <map>
#include <memory>
#include <set>
#include <algorithm>

namespace hd
{
    namespace filesystem
    {
        class FilesystemEntryList
        {
        private:
            std::map<fs::path, FilesystemEntry> entriesByPath, oldEntries;

        public:
            void add( const FilesystemEntry &entry, std::map<fs::path, FilesystemEntry> &addTo )
            {
                addTo[entry.path] = entry;
            }

            void fromXml( const std::string &xmlData )
            {
                namespace pt = boost::property_tree;
                pt::ptree tree;
                pt::read_xml( std::istringstream( xmlData ), tree );
                
                for( auto &entry : tree.get_child( "fel" ) )
                {
                    auto md5 = entry.second.get_child("md5").get_value( "" );
                    auto path = fs::path( entry.second.get_child( "path" ).get_value( "" ) );
                    auto modDate = entry.second.get_child( "moddate" ).get_value( "" );
                    auto typeStr = entry.second.get_child( "type" ).get_value( "" );

                    auto type = ( typeStr == "DIR" ? FilesystemEntryType::DIRECTORY : FilesystemEntryType::FILE );

                    add( { path, modDate, md5, type }, entriesByPath );
                }
            }

            std::string toXml() const 
            {
                namespace pt = boost::property_tree;

                pt::ptree tree;
                std::ostringstream oss;
                std::set<FilesystemEntry,
                         FilesystemEntry::SetPred> sorted;

                for( const auto &pair : entriesByPath )
                    sorted.insert( pair.second );



                // Put the simple values into the tree. The integer is automatically
                // converted to a string. Note that the "debug" node is automatically
                // created if it doesn't exist.
                //tree.put( "debug.filename", m_file );
                //tree.put( "debug.level", m_level );

                // Add all the modules. Unlike put, which overwrites existing nodes, add
                // adds a new node at the lowest level, so the "modules" node will have
                // multiple "module" children.
                /*BOOST_FOREACH( const std::string &name, m_modules )
                    tree.add( "debug.modules.module", name );*/

                for( const auto &entry : sorted )
                {
                    pt::ptree entryTree;

                    entryTree.put( "path", entry.path.string() );
                    entryTree.put( "md5", entry.md5 );
                    entryTree.put( "moddate", entry.modificationDate );
                    entryTree.put( "type", entry.stringType() );

                    tree.add_child( "fel.entry", entryTree );
                }

                // Write property tree to XML file
                pt::write_xml( oss, tree );

                return oss.str();
            }

            const auto findPath( const fs::path &path ) const
            {
                return entriesByPath.find( path );
            }

            bool pathExists( const fs::path &path ) const
            {
                return ( entriesByPath.find( path ) != entriesByPath.end() );
            }

            const auto end() const
            {
                return entriesByPath.end();
            }

            void generateOld( const std::string &path )
            {
                generate( path, &oldEntries );
            }

            void generate( const std::string &path, std::map<fs::path, FilesystemEntry> *out = nullptr )
            {
                if( out == nullptr )
                    out = &entriesByPath;

                out->clear();

                if( fs::exists( path )
                    && fs::is_directory( path ) )
                {
                    for( auto& p : fs::recursive_directory_iterator( path ) )
                        add( FilesystemEntry::create( p, path ), *out );
                }
            }

            std::vector<FilesystemEntryDifference> getDifferences( const FilesystemEntryList &other ) const 
            {
                std::vector<FilesystemEntryDifference> results;

                for( const auto &entryPair : entriesByPath )
                {
                    const auto &entry = entryPair.second;


                    if( other.pathExists( entry.path ) )// jak jest sciezka i tu i tu
                    {
                        const auto entryOther = other.findPath( entry.path )->second;

                        if( entry.md5 != entryOther.md5 ) // jak inne md5
                        {
                            if( entry.olderThan( entryOther ) ) // jak nasze starsze
                                results.push_back( { entry.path, DifferenceType::CHANGED_FILE_REMOTE } );
                            else // nasze mlodsze
                                results.push_back( { entry.path, DifferenceType::CHANGED_FILE_LOCALLY } );
                        }
                    }
                    else // jak nie ma sciezki na serwerze
                    {
                        if( oldEntries.find( entry.path ) == oldEntries.end() ) // jak u nas w starych nie bylo
                        {
                            if( entry.type == FilesystemEntryType::DIRECTORY )
                                results.push_back( { entry.path, DifferenceType::NEW_DIR_LOCALLY } );
                            else
                                results.push_back( { entry.path, DifferenceType::NEW_FILE_LOCALLY } );
                        }
                        else // jak byla
                        {
                            if( entry.type == FilesystemEntryType::DIRECTORY )
                                results.push_back( { entry.path, DifferenceType::DELETED_DIR_REMOTE } );
                            else
                                results.push_back( { entry.path, DifferenceType::DELETED_FILE_REMOTE } );
                        }
                    }
                    //jest sciezka i takie samo md5 => bez zmian
                    //jest sciezka i inne md5
                        //data other starsza => CHANGED_FILE_REMOTE
                        //data other mniejsza => CHANGED_FILE_LOCALLY
                    //W OTHER nie ma sciezki
                        //dwie opcje
                        //albo u nas dodano
                        //albo na serwerze usunieto
                        //jak w starym u nas nie ma
                            //u nas dodano
                        //jak jest
                            //na serwerze usunieto
                        //NEW_FILE_LOCALLY
                }

                for( const auto &entryPair : other.entriesByPath )
                {
                    const auto &entryOther = entryPair.second;
                    const auto entry = findPath( entryOther.path );

                    if( entry == end() ) // jak sciezki z serwera nie ma u nas
                    {
                        if( oldEntries.find( entryOther.path ) == oldEntries.end() ) // jak u nas w starych nie ma
                        {
                            if( entryOther.type == FilesystemEntryType::DIRECTORY )
                                results.push_back( { entryOther.path, DifferenceType::NEW_DIR_REMOTE } );
                            else
                                results.push_back( { entryOther.path, DifferenceType::NEW_FILE_REMOTE } );
                        }
                        else // jak u nas w starych jest
                        {
                            if( entryOther.type == FilesystemEntryType::DIRECTORY )
                                results.push_back( { entryOther.path, DifferenceType::DELETED_DIR_LOCALLY } );
                            else
                                results.push_back( { entryOther.path, DifferenceType::DELETED_FILE_LOCALLY } );
                        }
                    }
                }
                //for other
                    //nie ma sciezki u nas
                        //dwie opcje
                        //albo na serwerze doszlo
                        //albo u nas usunieto
                        //trzeba trzymac nasze jedno stare
                        //jak u nas w starym nie ma
                            //doszlo na serwerze
                        //jak jest
                            //usunieto u nas

                return results;
            }

            friend std::ostream& operator <<( std::ostream &out, const FilesystemEntryList &list )
            {
                for( const auto &entry : list.entriesByPath )
                    out << entry.second << "\n\n";

                return out;
            }
        };
    }
}