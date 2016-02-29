package stryku.homediscserver;

/**
 * Created by stryku on 14.02.16.
 */
public class ResponseBuilder {
    private String file = null;
    private String path = null;

    public ResponseBuilder setFilesystemEntryList(String filesystemEntryList) {
        this.filesystemEntryList = filesystemEntryList;
        return this;
    }

    private String filesystemEntryList = null;
    private boolean succeed = true;

    public ResponseBuilder setSucceed(boolean succeed) {
        this.succeed = succeed;
        return this;
    }

    public ResponseBuilder setPath(String path) {
        this.path = path;
        return this;
    }

    public ResponseBuilder setFile(String file) {
        this.file = file;
        return this;
    }

    public String build() {
        StringBuilder response = new StringBuilder();

        response.append("<resp>");

        response.append(String.format("<succeed>%s</succeed>", String.valueOf(succeed)));

        if(path != null)
            response.append(String.format("<path>%s</path>", path));

        if(file != null)
            response.append(String.format("<file>%s</file>", file));

        if(filesystemEntryList != null)
            response.append(String.format("<fel>%s</fel>", filesystemEntryList));

        response.append("</resp>");

        return response.toString();
    }
}
