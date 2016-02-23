package stryku.homediscserver;

import java.io.File;
import java.text.SimpleDateFormat;

/**
 * Created by stryku on 14.02.16.
 */
public class FilesystemEntry {
    public enum FilesystemEntryType {
        FILE,
        DIRECTORY,
        UNDEF
    }

    public String path;
    public String modificationDate;
    public String md5 = "";
    public FilesystemEntryType type;

    FilesystemEntry(File file) {
        this.path = file.getPath().substring(Settings.getMainFolderPath().length() + 1);

        if(file.isFile())
            this.md5 = IOUtil.md5OfFile(file.getPath());


        if(file.isDirectory())
            this.type = FilesystemEntryType.DIRECTORY;
        else
            this.type = FilesystemEntryType.FILE;

        SimpleDateFormat sdf = new SimpleDateFormat("yyyyMMddHHmmss");
        this.modificationDate = sdf.format(file.lastModified());
    }

    public String toXml() {
        String xml = "";

        xml += String.format("<path>%s</path>", path);
        xml += String.format("<md5>%s</md5>", md5);
        xml += String.format("<moddate>%s</moddate>", modificationDate);
        xml += String.format("<type>%s</type>", type.toString());

        return xml;
    }
}
