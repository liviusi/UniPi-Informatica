import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class Esercizio
{

	private static List<File> rec_listFiles(List<File> listFiles, File f, BufferedWriter bwDirectories)
	{
		if (!f.isDirectory())
		{
			listFiles.add(f);
			return listFiles;
		}
		else
		{
			try
			{
				bwDirectories.append(f.getCanonicalPath());
				bwDirectories.newLine();
			}
			catch (IOException e) { }
		}
		for (File i : f.listFiles()) rec_listFiles(listFiles, i, bwDirectories);
		return listFiles;
	}

	public static void listFiles(String root)
	{
		File rootFile = new File(root);
		if (!rootFile.isDirectory())
		{
			System.err.println(root + " is not a directory.");
			return;
		}
		try
		{
			BufferedWriter bwDirectories = new BufferedWriter(new FileWriter(new File("./directories.txt").getCanonicalPath(), true));
			BufferedWriter bwFiles = new BufferedWriter(new FileWriter(new File("./listFiles.txt").getCanonicalPath(), true));
			List<File> listFiles = rec_listFiles(new ArrayList<File>(), rootFile, bwDirectories);
			for (File f : listFiles)
			{
				bwFiles.append(f.getCanonicalPath());
				bwFiles.newLine();
			}
			bwFiles.close();
			bwDirectories.close();
		}
		catch (IOException e) { }
	}
	public static void main(String[] args)
	{
		if (args.length != 1) { System.err.println("java Esercizio <path/to/dir>"); return; }

		listFiles(args[0]);
	}
}