package stryku.homediscserver;

import android.util.Xml;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;

import java.io.IOException;
import java.io.InputStream;

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
            return parseRequest(parser);
        } finally {
            in.close();
        }
    }

    private Request parseRequest(XmlPullParser parser) throws XmlPullParserException, RequestXmlParserException, IOException {
        Request request = null;
        String text = null;

        int eventType = parser.getEventType();

        while (eventType != XmlPullParser.END_DOCUMENT) {
            String tagname = parser.getName();
            switch (eventType) {
                case XmlPullParser.START_TAG:
                    if (tagname.equalsIgnoreCase("request"))
                        request = new Request();
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
}
