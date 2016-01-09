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
            NEW_LOCALLY,
            NEW_OTHER,
            DELETED_LOCALLY,
            DELETED_OTHER,
            NAME_CHANGED,
            CHANGED_OTHER,
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
                    case DifferenceType::NEW_LOCALLY: return "NEW_LOCALLY"; break;
                    case DifferenceType::NEW_OTHER: return "NEW_OTHER"; break;
                    case DifferenceType::DELETED_LOCALLY: return "DELETED_LOCALLY"; break;
                    case DifferenceType::DELETED_OTHER: return "DELETED_OTHER"; break;
                    case DifferenceType::NAME_CHANGED: return "NAME_CHANGED"; break;
                    case DifferenceType::CHANGED_OTHER: return "CHANGED_OTHER"; break;
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