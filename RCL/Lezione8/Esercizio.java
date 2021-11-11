import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.util.HashMap;
import java.util.Map;

/**
Scrivere un programma che legge il file di testo random.txt (in allegato) e scrive su un file di output
la frequenza dei caratteri alfabetici contenuti nel testo. Lettura e scrittura devono essere
effettuate utilizzando NIO, in particolare le classi FileChannel e ByteBuffer.
Non è necessario distinguere fra caratteri maiuscoli o minuscoli.
Nota: dato il file in ingresso, potete assumere che ciascun byte che leggete dal buffer rappresenti
un carattere.
 */


public class Esercizio
{
	public static final String INPUT_FILENAME = "random.txt";
	public static final String OUTPUT_FILENAME = "frequency.txt";
	public static final int BUFFER_SIZE = 4096;
	public static void main(String[] args)
	{
		Map<Character, Integer> frequency = new HashMap<>();
		for (char c = 'a'; c <= 'z'; c++) frequency.put(c, 0); // initialize frequency hashmap
		try
		(
			final FileInputStream fInputStream = new FileInputStream(INPUT_FILENAME);
			final FileChannel fInputChannel = fInputStream.getChannel();
			final FileOutputStream fOutputStream = new FileOutputStream(OUTPUT_FILENAME);
			final FileChannel fOutputChannel = fOutputStream.getChannel()
		)
		{
			ByteBuffer byteBuffer = ByteBuffer.allocate(BUFFER_SIZE);
			while (fInputChannel.read(byteBuffer) != -1) // buffer is in write mode
			{
				byteBuffer.flip(); // buffer is in read mode
				while (byteBuffer.hasRemaining()) // empty it out
				{
					char c = (char) byteBuffer.get(); // every byte represents a single character per requirements
					if (!Character.isLetter(c)) continue;
					c = Character.toLowerCase(c);
					frequency.put(c, frequency.get(c) + 1);
				}
				byteBuffer.clear();
			}
			for (var entry: frequency.entrySet())
			{
				String line = entry.getKey() + " - " + entry.getValue() + "\n";
				byteBuffer.clear();
				byteBuffer.put(line.getBytes()); // buffer is in write mode
				byteBuffer.flip(); // buffer is in read mode
				while (byteBuffer.hasRemaining()) fOutputChannel.write(byteBuffer);
			}
		}
		catch (FileNotFoundException e)
		{
			System.err.printf("File could not be found:\n%s", e.getMessage());
			System.exit(1);
		}
		catch (IOException e)
		{
			System.err.printf("I/O error:\n%s", e.getMessage());
			System.exit(1);
		}
	}
}
