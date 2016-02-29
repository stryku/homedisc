package stryku.homediscserver;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

/**
 * Created by stryku on 22.02.16.
 */
public class MD5Checksum {
    public static byte[] createChecksum(File file) throws IOException, NoSuchAlgorithmException {
        InputStream fis =  new FileInputStream(file.getPath());

        byte[] buffer = new byte[1024];
        MessageDigest complete = MessageDigest.getInstance("MD5");
        int numRead;

        do {
            numRead = fis.read(buffer);
            if (numRead > 0) {
                complete.update(buffer, 0, numRead);
            }
        } while (numRead != -1);

        fis.close();
        return complete.digest();
    }

    public static String getMD5Checksum(String path) throws IOException, NoSuchAlgorithmException {
        return getMD5Checksum(new File(path));
    }

    public static String getMD5Checksum(File file) throws IOException, NoSuchAlgorithmException {
        byte[] bytes = createChecksum(file);

        StringBuilder sb = new StringBuilder();

        for(int i=0; i< bytes.length ;i++)
            sb.append(Integer.toString((bytes[i] & 0xff) + 0x100, 16).substring(1));

        return sb.toString();
    }
}
