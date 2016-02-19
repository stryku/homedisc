package stryku.homediscserver;

import android.util.Xml;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;

import java.io.IOException;
import java.io.InputStream;

import zmq.Req;

/**
 * Created by stryku on 14.02.16.
 */
public class RequestXmlParser {
    private static final String ns = null;

    public class RequestXmlParserException extends Throwable {
        RequestXmlParserException(String msg) {
            super(msg);
        }
    }

    public Request parse(InputStream in) throws XmlPullParserException, IOException, RequestXmlParserException {
        try {
            XmlPullParser parser = Xml.newPullParser();
            parser.setFeature(XmlPullParser.FEATURE_PROCESS_NAMESPACES, false);
            parser.setInput(in, null);
            //parser.require(XmlPullParser.START_TAG, ns, "request");
            return parseRequest2(parser);
        } finally {
            in.close();
        }
    }

    private Request parseRequest2(XmlPullParser parser) throws XmlPullParserException, RequestXmlParserException, IOException {
        Request request = null;
        String text = null;

        int eventType = parser.getEventType();

        while (eventType != XmlPullParser.END_DOCUMENT) {
            String tagname = parser.getName();
            switch (eventType) {
                case XmlPullParser.START_TAG:
                    if (tagname.equalsIgnoreCase("request")) {
                        // create a new instance of employee
                        request = new Request();
                    }
                    break;

                case XmlPullParser.TEXT:
                    text = parser.getText();
                    break;

                case XmlPullParser.END_TAG:
                    if (tagname.equalsIgnoreCase("path")) {
                        request.values.put("path", text);
                    } else if (tagname.equalsIgnoreCase("type")) {
                        request.type = Request.Type.valueOf(text);
                    } else if (tagname.equalsIgnoreCase("file")) {
                        request.values.put("file", text);
                    } else if (tagname.equalsIgnoreCase("request")) {
                        return request;
                    } else {
                        throw new RequestXmlParserException("Broken request. Unexpected <" + tagname.toString() + "> tag");
                    }
                    break;

                default:
                    break;
            }
            eventType = parser.next();
        }

        return request;
    }

    static private Request parseRequest(XmlPullParser parser) throws IOException, XmlPullParserException {
        Request req = new Request();

        req.type = readType(parser);

        readRestOfRequest(parser, req);

        return req;
    }

    static private String readTag(XmlPullParser parser, String tagName) throws IOException, XmlPullParserException {
        parser.require(XmlPullParser.START_TAG, ns, tagName);
        String tag = readText(parser);
        parser.require(XmlPullParser.END_TAG, ns, tagName);

        return tag;
    }

    static private Request.Type readType(XmlPullParser parser) throws IOException, XmlPullParserException {
        String type = readTag(parser, "type");
        return Request.Type.valueOf(type);
    }

    static private String readPath(XmlPullParser parser) throws IOException, XmlPullParserException {
        return readTag(parser, "path");
    }

    static private String readFile(XmlPullParser parser) throws IOException, XmlPullParserException {
        return readTag(parser, "file");
    }

    static private void readRestOfRequest(XmlPullParser parser, Request req) throws IOException, XmlPullParserException {
        switch (req.type) {
            case LIST_REQ:
                break;

            case FILE_REQ:
            case REMOVE_FILE:
            case REMOVE_DIR:
            case NEW_DIR:
                req.values.put("path", readPath(parser));
                break;

            case NEW_FILE:
                req.values.put("path", readPath(parser));
                req.values.put("file", readFile(parser));
                break;
        }
    }

    static private String readText(XmlPullParser parser) throws IOException, XmlPullParserException {
        String result = "";
        if (parser.next() == XmlPullParser.TEXT) {
            result = parser.getText();
            parser.nextTag();
        }
        return result;
    }
}
