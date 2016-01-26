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
            NEW_FILE_LOCALLY,
            NEW_DIR_LOCALLY,
            NEW_FILE_REMOTE,
            NEW_DIR_REMOTE,
            DELETED_FILE_LOCALLY,
            DELETED_DIR_LOCALLY,
            DELETED_FILE_REMOTE,
            DELETED_DIR_REMOTE,
            NAME_CHANGED,
            CHANGED_FILE_REMOTE,
            CHANGED_DIR_REMOTE,
            CHANGED_FILE_LOCALLY,
            CHANGED_DIR_LOCALLY
        };

        struct FilesystemEntryDifference
        {
            fs::path entryPath;
            DifferenceType type;

            std::string stringType() const
            {
                switch( type )
                {
                    case DifferenceType::NEW_FILE_LOCALLY: return "NEW_FILE_LOCALLY"; break;
                    case DifferenceType::NEW_FILE_REMOTE: return "NEW_FILE_REMOTE"; break;
                    case DifferenceType::DELETED_FILE_LOCALLY: return "DELETED_FILE_LOCALLY"; break;
                    case DifferenceType::DELETED_FILE_REMOTE: return "DELETED_FILE_REMOTE"; break;
                    case DifferenceType::NAME_CHANGED: return "NAME_CHANGED"; break;
                    case DifferenceType::CHANGED_FILE_REMOTE: return "CHANGED_FILE_REMOTE"; break;
                    case DifferenceType::CHANGED_FILE_LOCALLY: return "CHANGED_FILE_LOCALLY"; break;
                    default: return "UNKNOW"; break;
                }
            }

            friend std::ostream& operator<<( std::ostream &out, const FilesystemEntryDifference &dif )
            {
                out << "path: " << dif.entryPath << "\n" \
                    << "type: " << dif.stringType();

                return out;
            }

        };
    }
}