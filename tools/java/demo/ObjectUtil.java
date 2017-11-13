
// https://stackoverflow.com/questions/6055476/how-to-convert-object-to-string-in-java

package pt.iol.security;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.zip.GZIPInputStream;
import java.util.zip.GZIPOutputStream;
import org.apache.commons.codec.binary.Base64;

public class ObjectUtil {

	static final Base64 base64 = new Base64();

	public static String serializeObjectToString(Object object) throws IOException {
		try (
			ByteArrayOutputStream arrayOutputStream = new ByteArrayOutputStream();
			GZIPOutputStream gzipOutputStream = new GZIPOutputStream(arrayOutputStream);
			ObjectOutputStream objectOutputStream = new ObjectOutputStream(gzipOutputStream);) {
			objectOutputStream.writeObject(object);
			objectOutputStream.flush();
			return new String(base64.encode(arrayOutputStream.toByteArray()));
		}
	}

	public static Object deserializeObjectFromString(String objectString) throws IOException, ClassNotFoundException {
		try (
			ByteArrayInputStream arrayInputStream = new ByteArrayInputStream(base64.decode(objectString));
			GZIPInputStream gzipInputStream = new GZIPInputStream(arrayInputStream);
			ObjectInputStream objectInputStream = new ObjectInputStream(gzipInputStream)) {
			return objectInputStream.readObject();
		}
	}
}
