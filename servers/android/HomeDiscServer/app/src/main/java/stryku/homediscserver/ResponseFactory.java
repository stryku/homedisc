package stryku.homediscserver;

/**
 * Created by stryku on 14.02.16.
 */
public class ResponseFactory {
    static public String getFileResponse(boolean succeed, String path, String file) {
        ResponseBuilder rb = new ResponseBuilder();
        return rb.setSucceed(succeed)
                .setPath(path)
                .setFile(file)
                .build();
    }

    static public String getSucceedResponse() {
        ResponseBuilder rb = new ResponseBuilder();
        return rb.setSucceed(true)
                .build();
    }

    static public String getFailResponse() {
        ResponseBuilder rb = new ResponseBuilder();
        return rb.setSucceed(false)
                .build();
    }

    static public String getListResponse(String stringList) {
        ResponseBuilder rb = new ResponseBuilder();
        return rb.setFilesystemEntryList(stringList)
                .build();
    }
}
