package stryku.homediscserver;

import android.util.Base64;

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.RandomAccessFile;
import java.io.UnsupportedEncodingException;
import java.math.BigInteger;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

/**
 * Created by stryku on 14.02.16.
 */
public class IOUtil {
    public static byte[] readFile(String file) throws IOException {
        return readFile(new File(file));
    }

    public static byte[] readFile(File file) throws IOException {
        RandomAccessFile f = new RandomAccessFile(file, "r");
        try {
            long longlength = f.length();
            int length = (int) longlength;

            if (length != longlength)
                throw new IOException("File size >= 2 GB");

            byte[] data = new byte[length];

            f.readFully(data);

            return data;
        } finally {
            f.close();
        }
    }

    static public String readFileToString(String path) throws IOException {
        byte[] bytes = readFile(path);
        return new String(bytes);
    }

    static public String md5OfFile(String path) {
        String result = null;

        try {
            result = MD5Checksum.getMD5Checksum(path);
        } catch (NoSuchAlgorithmException e) {
            System.out.println("Exception while encrypting to md5");
            e.printStackTrace();
            return null;
        } catch (IOException e) {
            System.out.println("Exception while encrypting to md5");
            e.printStackTrace();
            return null;
        } catch (Exception e) {
            e.printStackTrace();
        }

        /*mdEnc.update(fileContent.getBytes(), 0, fileContent.length());
        String md5 = new BigInteger(1, mdEnc.digest()).toString(16);
        while ( md5.length() < 32 ) {
            md5 = "0"+md5;
        }
        return md5;*/
        return result;
    }

    static public String fileToBase64(String path) throws IOException {
        String b64;
        byte[] fileBytes = readFile(path);

        return Base64.encodeToString(fileBytes, Base64.DEFAULT);
    }

    static public boolean base64ToFile(String path, String base64) {
        File file=new File(path);
        byte[] decoded = Base64.decode(base64, Base64.DEFAULT);

        if (file.exists())
            file.delete();

        try {
            FileOutputStream fos=new FileOutputStream(path);

            fos.write(decoded);
            fos.close();

            return true;
        }
        catch (java.io.IOException e) {
            e.printStackTrace();
            return false;
        }
    }


    static int deleteRecursive(String fileOrDirectory) {
        return deleteRecursive(new File(fileOrDirectory));
    }

    static int deleteRecursive(File fileOrDirectory) {
        int deletedEntries = 0;

        if (fileOrDirectory.isDirectory()) {
            for (File child : fileOrDirectory.listFiles()) {
                deletedEntries += deleteRecursive(child);
            }
        }

        if(fileOrDirectory.delete())
            ++deletedEntries;

        return deletedEntries;
    }
}
