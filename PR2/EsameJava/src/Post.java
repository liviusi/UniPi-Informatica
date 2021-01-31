public interface Post 
{
    /* 
    OVERVIEW:
        Interfaccia implementata dalla classe MyPost.
        Il parametro maxlen - immodificabile e condiviso da ogni istanza
        di post - serve per verificare alcune condizioni sui suoi parametri
        di istanza.
    
    TYPICAL ELEMENT:
        {id, author, text, timestamp} stringhe.
    */

    static final int maxlen = 140;

    // Getter per l'identificatore univoco del post.
    public String getID();
    /*
    REQUIRES:
        ---.
    THROWS:
        ---.
    MODIFIES:
        ---.
    EFFECTS:
        Restituisce l'ID del post.
    */

    // Getter per la stringa che identifica l'autore del post.
    public String getAuthor();
    /*
    REQUIRES:
        ---.
    THROWS:
        ---.
    MODIFIES:
        ---.
    EFFECTS:
        Restituisce l'autore del post.
    */

    // Getter per il testo del post.
    public String getText();
    /*
    REQUIRES:
        ---.
    THROWS:
        ---.
    MODIFIES:
        ---.
    EFFECTS:
        Restituisce il testo del post. 
    */

    // Getter per il timestamp del post.
    public String getTimestamp();
    /*
    REQUIRES:
        ---.
    THROWS:
        ---.
    MODIFIES:
        ---.
    EFFECTS:
        Restituisce data e ora di invio del post.
    */

    // Unchecked Exception usata ogni volta che si prova a istanziare un post illegale.
    public class IllegalPostException extends Exception
    {
        private static final long serialVersionUID = 1L;

        public IllegalPostException() {
            super();
        }

        public IllegalPostException(String s) { super(s); }
    }
}
