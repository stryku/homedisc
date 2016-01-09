#pragma once

#include <filesystem>
#include <string>

namespace hd
{
    namespace filesystem
    {
        namespace fs = std::experimental::filesystem;

        enum class DifferenceType
        {
            NEW,
            DELETED,
            NAME_CHANGED,
            CHANGED_ON_SERVER,
            CHANGED_LOCALLY
        };

        struct FilesystemEntryDifference
        {
            fs::path entryPath;
            DifferenceType type;

            std::string stringType() const
            {
                switch( type )
                {
                    case DifferenceType::NEW: return "NEW"; break;
                    case DifferenceType::DELETED: return "DELETED"; break;
                    case DifferenceType::NAME_CHANGED: return "NAME_CHANGED"; break;
                    case DifferenceType::CHANGED_ON_SERVER: return "CHANGED_ON_SERVER"; break;
                    case DifferenceType::CHANGED_LOCALLY: return "CHANGED_LOCALLY"; break;
                    default: return "UNKNOW"; break;
                }
            }

            friend std::ostream& operator<<( std::ostream &out, const FilesystemEntryDifference &dif )
            {
                out << "path: " << dif.path << "\n" \
                    << "type: " << dif.stringType();

                return out;
            }

        };
    }
}