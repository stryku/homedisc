#pragma once

#include "FilesystemEntry.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <unordered_set>
#include <sstream>

namespace hd
{
    namespace filesystem
    {
        class FilesystemEntryList
        {
        private:
            std::unordered_set<FilesystemEntry> entries;

        public:
            void add( const FilesystemEntry &entry )
            {
                entries.insert( entry );
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

                    add( { path, modDate, md5, type } );
                }
            }

            std::string toXml()
            {
                namespace pt = boost::property_tree;

                pt::ptree tree;

                std::ostringstream oss;

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

                for( const auto &entry : entries )
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

            friend std::ostream& operator <<( std::ostream &out, const FilesystemEntryList &list )
            {
                for( const auto &entry : list.entries )
                    out << entry << "\n\n";

                return out;
            }
        };
    }
}