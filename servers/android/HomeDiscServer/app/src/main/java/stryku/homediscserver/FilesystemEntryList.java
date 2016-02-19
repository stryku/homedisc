package stryku.homediscserver;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by stryku on 14.02.16.
 */
public class FilesystemEntryList {
    List<FilesystemEntry> entries = new ArrayList<FilesystemEntry>();

    FilesystemEntryList(String path) {
        generate(path);
    }

    private void addRecursive(File fileOrDirectory) {
        if (fileOrDirectory.isDirectory()) {
            for (File child : fileOrDirectory.listFiles()) {
                addRecursive(child);
            }
        }

        if(fileOrDirectory.getPath() != Settings.getMainFolderPath())
            entries.add(new FilesystemEntry(fileOrDirectory));
    }

    public void generate(String path) {
        addRecursive(new File(path));
    }

    public String toXml() {
        String xml = new String();

        for(FilesystemEntry entry : entries) {

            xml += "<entry>" + entry.toXml() + "</entry>";
        }


        return xml;
    }
}
