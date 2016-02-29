package stryku.homediscserver;

import android.os.Handler;
import android.os.Message;
import android.util.Log;

import org.xmlpull.v1.XmlPullParserException;
import org.zeromq.ZMQ;

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.LinkedBlockingQueue;

/**
 * Created by stryku on 14.02.16.
 */
public class RequestHandler implements Runnable {
    private LinkedBlockingQueue<PersonalMessage> requestsToHandle = new LinkedBlockingQueue<PersonalMessage>();
    private MessageSender sender;
    private List<Handler> eventHandlers = new ArrayList<>();


    public void addEventHandler(Handler eventHandler) {
        eventHandlers.add(eventHandler);
    }

    private String getFullPath(Request req) {
        String relativePath = req.values.get("path");
        String mainFolderPath = Settings.getMainFolderPath();

        return String.format("%s/%s", mainFolderPath, relativePath);
    }

    private String handleFileRequest(Request req) {
        String fullPath = getFullPath(req);
        String relativePath = req.values.get("path");
        String b64File = null;

        try {
            b64File = IOUtil.fileToBase64(fullPath);
            emitEvent(createEventMsg(req.type, true, req.values.get("path")));
            return ResponseFactory.getFileResponse(true, relativePath, b64File);
        } catch (IOException e) {
            e.printStackTrace();
        }

        emitEvent(createEventMsg(req.type, false, req.values.get("path")));
        return ResponseFactory.getFailResponse();
    }

    private String handleRemoveFileOrDirectoryRequest(Request req) {
        String fullPath = getFullPath(req);

        if (IOUtil.deleteRecursive(fullPath) == 0) {
            emitEvent(createEventMsg(req.type, false, req.values.get("path")));
            return ResponseFactory.getFailResponse();
        }

        emitEvent(createEventMsg(req.type, true, req.values.get("path")));
        return ResponseFactory.getSucceedResponse();
    }

    private String createEventMsg(Request.Type requestType, boolean succeed, String path ) {
        return String.format("[%s][%s]: %s\n", requestType.toString(), String.valueOf(succeed), path);
    }

    private String handleNewDirectory(Request req) {
        String fullPath= getFullPath(req);
        File dir = new File(fullPath);
        String eventMsg;

        if(dir.mkdir()) {
            emitEvent(createEventMsg(req.type, true, req.values.get("path")));
            return ResponseFactory.getSucceedResponse();
        }
        else {
            emitEvent(createEventMsg(req.type, false, req.values.get("path")));
            return ResponseFactory.getFailResponse();
        }
    }

    private String handleNewFile(Request req) {
        String fullPath = getFullPath(req);
        String b64File = req.values.get("file");
        String eventMsg;


        if (IOUtil.base64ToFile(fullPath, b64File)) {
            emitEvent(createEventMsg(req.type, true, req.values.get("path")));
            return ResponseFactory.getSucceedResponse();

        }

        emitEvent(createEventMsg(req.type, false, req.values.get("path")));
        return ResponseFactory.getFailResponse();
    }

    private String handleListRequest(Request req) {
        FilesystemEntryList fel = new FilesystemEntryList(Settings.getMainFolderPath());
        emitEvent(createEventMsg(req.type, true, ""));
        return ResponseFactory.getListResponse(fel.toXml());
    }

    private String handleRequest(Request req) {
        String response = null;

        switch (req.type) {
            case LIST_REQ:
                response = handleListRequest(req);
                break;
            case FILE_REQ:
                response = handleFileRequest(req);
            break;
            case REMOVE_FILE:
            case REMOVE_DIR:
                response = handleRemoveFileOrDirectoryRequest(req);
                break;
            case NEW_FILE:
                response = handleNewFile(req);
                break;
            case NEW_DIR:
                response = handleNewDirectory(req);
                break;
        }

        return response;
    }

    private void handlePersonalMessage( PersonalMessage personalMsg ) {
        Log.d("DEB", "Handling request");
        try {
            InputStream in = new ByteArrayInputStream(personalMsg.msg.getBytes(ZMQ.CHARSET));
            RequestXmlParser parser = new RequestXmlParser();
            Request req = parser.parse(in);
            String strResponse = handleRequest(req);
            PersonalMessage response = new PersonalMessage(personalMsg.identity, strResponse);
            Log.d("DEB", "Request handled. Sending response: " + response.toString());
            sender.newMessageToSend(response);
        } catch (XmlPullParserException e) {
            e.printStackTrace();
            Log.d("DEB", "Failed parsing request: " + e.toString());
        } catch (IOException e) {
            e.printStackTrace();
            Log.d("DEB", "Failed handling request: ioexception");
        } catch (RequestXmlParser.RequestXmlParserException e) {
            e.printStackTrace();
            Log.d("DEB", "Failed parsing request: " + e.toString());
        }
    }

    @Override
    public void run() {
        Log.d("DEB", "Request handler started");
        while( true ) {
            try {
                Log.d("DEB", "Waiting for new request");
                PersonalMessage msg = requestsToHandle.take();
                Log.d("DEB", "Received new request: " + msg.toString());
                handlePersonalMessage(msg);
            } catch (InterruptedException e) {
                e.printStackTrace();
                Log.d("DEB", "Handler thread interrupted");
            }
        }
    }

    public void newRequest( PersonalMessage req ) {
        try {
            requestsToHandle.put( req );
        } catch( InterruptedException e ) {
            e.printStackTrace();
        }
    }

    public void setSender(MessageSender sender) {
        this.sender = sender;
    }

    private void emitEvent(String eventMsg) {
        Message msg = new Message();

        msg.obj = eventMsg;

        for(Handler handler : eventHandlers)
            handler.sendMessage(msg);
    }
}
