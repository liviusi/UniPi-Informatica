import java.text.SimpleDateFormat;
import java.util.Date;

public class MyPost implements Post
{
    /*
    OVERVIEW:
        Collezione immutabile di dati. Gli elementi caratteristici sono
        ID := identificatore univoco del post;
        Author := utente (una stringa non contenente spazi) della rete sociale che ha scritto il post;
        Text := testo (massimo 140 caratteri) del post;
        Timestamp := data e ora di invio del post.

    TYPICAL ELEMENT:
        { ID, Author, Text, Timestamp }.

    AF:
        Funzione identità.

    IR:
        (ID >= 0 && Author != null && !Author.contains(" ")
        && Text != null && Text.length < maxlen && Timestamp != null)
    */

    private final String ID;
    private final String Author;
    private final String Text;
    private final String Timestamp;

    // Costruttore della classe MyPost a cui si passa ogni parametro.
    public MyPost(long ID, String Author, String Text) 
        throws NullPointerException, IllegalPostException
    {
        if (ID < 0) throw new IllegalPostException("ID needs to be a positive integer.\n");
        this.ID = Long.toString(ID);
        if (Author == null) throw new NullPointerException("Author cannot be null.\n");
        if (Author.isEmpty()) throw new IllegalPostException("Author cannot be empty.\n");
        if (Author.contains(" ")) throw new IllegalPostException("Author cannot contain whitespaces.\n");
        this.Author = Author;
        if (Text == null) throw new NullPointerException("Text cannot be null.\n");
        else if (Text.isEmpty()) throw new IllegalPostException("Text cannot be empty\n");
        else if (Text.length() > maxlen) throw new IllegalPostException("Text must be - at most - made up of " + maxlen
            + " characters.\n");
        this.Text = Text;
        this.Timestamp = new SimpleDateFormat("dd-MM-yyyy HH:mm:ss").format(new Date());
    }
    /*
    REQUIRES:
        ID >= 0  && Author != null && !Author.contains(" ") && !Author.isEmpty() &&
        && Text != null && !Text.isEmpty() && Text.length < maxlen.
    THROWS:
        NullPointerException (unchecked) se uno tra Author e Text vale null,
        IllegalPostException (checked) se ID < 0, Author.matches("/^\s+$/"), Author.isEmpty(), Text.isEmpty() o Text.length > 140.
    MODIFIES:
        this.
    EFFECTS:
        Istanzia un nuovo post coi parametri passati in input.
    */

    public String getID() { return this.ID; }

    public String getAuthor() { return this.Author; }

    public String getText() { return this.Text; }

    public String getTimestamp() { return this.Timestamp; }

    @Override
    public String toString() { return "ID: " + getID() + "\nAUTHOR: " +  getAuthor() + "\nTEXT: " +  getText() + "\nTIMESTAMP: " +  getTimestamp() + "\n"; }
}