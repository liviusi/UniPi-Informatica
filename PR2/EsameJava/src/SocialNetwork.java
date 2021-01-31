import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.regex.Pattern;

public interface SocialNetwork 
{
    /*
    OVERVIEW:
        Interfaccia implementata dalla classe MySocialNetwork.

    TYPICAL ELEMENT: 
        {(utente1, {utente_seguito1, ..., utente_seguiton}), ..., 
            (utenten, {utente_seguito1, utente_seguiton})},
        {post1, ..., postn},
        {like1, ..., liken}.
        Gli utenti sono identificati da stringhe, l'insieme degli utenti coincide con l'insieme degli
        autori dei post presenti nella rete sociale.

    NOTAZIONE:
        - Si definisce Posts := insieme dei post nella rete sociale.
        - Si definisce textualPosts := insieme dei post che non appartengono a un sottotipo di Post
        (i.e. che non sono like e/o segnalazioni).
        - Si definisce likePosts := insieme dei post di tipo like.
        - Si definisce Users := insieme degli utenti presenti nella rete sociale, ossia l'insieme degli autori presenti in Posts.
        - Si definisce la funzione ausiliaria getReferencedID(String s) che estrae l'identificativo
        del post a cui si fa riferimento all'interno della stringa s.
        - Si definisce la funzione ausiliaria LikePattern(String s) che verifica che il format di una stringa
        sia quello di un Like.
        LikePattern(s) restituisce true sse s è del tipo "Like: <int>" (case insensitive).
        - Si definisce la funzione ausiliaria ValidLike(Post p) che verifica se un post
        di tipo Like sia valido.
        ValidLike(p) restituisce true sse LikePattern(p.getText()) && Exists(Post q in textualPosts | getRefencedID(p.getText()).equals(q.getID()) && 
            !q.getAuthor().equals(p.getAuthor())) && !Exists(Post q in likePosts | getReferencedID(q.getText()).equals(getReferencedID(p.getText())) 
            && q.getAuthor().equals(p.getAuthor())).
    */

    // Aggiunge il post istanziato coi parametri in input a quelli presenti all'interno del social network.
    public Post addPost(String author, String text)
        throws NullPointerException, Post.IllegalPostException;
    /*
    REQUIRES:
        Post.requires() && LikePattern(text) => ValidLike(p),
        con p := oggetto di tipo Post | p.getAuthor().equals(author) && p.getText().equals(text).
    THROWS:
        NullPointerException (unchecked) se p vale null,
        IllegalPostException (checked) se il post non rispetta l'invariante di rappresentazione della propria classe
        o se vale LikePattern(text) ma non ValidLike(p).
    MODIFIES:
        this.
    EFFECTS:
        Istanzia un post coi parametri in input e lo inserisce nell'insieme dei post presenti nel social network.
        Formalmente:
        (!ValidLike(p) => post(textualPosts) = pre(textualPosts) U {p})
        && p is not in likePosts && ValidLike(p) =>
            --------------------------- [NOTAZIONE AGGIUNTIVA] --------------------------------------
            | a := post in textualPosts con identificativo getReferencedID(p.getText()).            |
            | follows[p.getAuthor()] := insieme degli utenti seguiti dall'autore del post p.        |
            | followers[a.getAuthor()] :=  insieme degli utenti che seguono l'autore del post a.    |
            -----------------------------------------------------------------------------------------
            (post(follows[p.getAuthor()]) = pre(follows[p.getAuthor()]) U {a.getAuthor()} &&
            post(followers[a.getAuthor()]) = pre(followers[a.getAuthor()]) U {p.getAuthor()} &&
            post(likes) = pre(likes) U {p}).
    */

    // Restituisce gli utenti più influenti delle rete sociale (parametro del metodo),
    // ovvero quelli che hanno un numero maggiore di “follower”.
    public List<String> influencers();
    /*
    REQUIRES:
        ---.
    THROWS:
        ---.
    MODIFIES:
        ---.
    EFFECTS:
        Ordina in modo decrescente le coppie (utenti, numero di followers)
        e restituisce la lista ordinata formata dagli utenti.
    */

    // Restituisce l’insieme degli utenti menzionati (inclusi) 
    // nei post presenti nella rete sociale.
    public Set<String> getMentionedUsers();
    /*
    REQUIRES:
        ---.
    THROWS:
        ---.
    MODIFIES:
        ---.
    EFFECTS:
        Itera sui testi dei post presenti nel social network: per ciascuno di essi, separa il testo in sottostringhe
        delimitate da uno spazio vuoto e restituisce l'insieme formato da tutte le sottostringhe
        immediatamente precedute dal carattere "@" (che non viene incluso) a patto che questa
        rappresenti un utente presente nella rete sociale.
        Formalmente:
        res := insieme di stringhe in output, inizialmente definito come insieme vuoto.
        for all p in Posts. 
        tokens := vettore di stringhe definito come p.getText().split("[ ]");
        for all t in tokens.
        (t.startswith("@") && t.substring(1) is in Users => (post(res) = pre(res) U {t.substring(1)}).
*/

    // Restituisce l’insieme degli utenti menzionati (inclusi) nella lista di post
    public Set<String> getMentionedUsers(List<Post> ps)
        throws NullPointerException;
    /*
    REQUIRES:
        ps != null && for all p in ps. p != null.
    THROWS:
        NullPointerException (unchecked) se ps vale null o se esiste un post che vale null all'interno della lista.
    MODIFIES:
        ---.
    EFFECTS:
        Itera sui testi dei post presenti nella lista passata in input: per ciascuno di essi, separa il testo in sottostringhe
        delimitate da uno spazio vuoto e restituisce l'insieme formato da tutte le sottostringhe
        immediatamente precedute dal carattere "@" (che non viene incluso) a patto che questa
        rappresenti un utente presente nella rete sociale.
        Formalmente:
        res := insieme di stringhe in output, inizialmente definito come insieme vuoto.
        for all p in ps. 
        tokens := vettore di stringhe definito come p.getText().split("[ ]");
        for all t in tokens.
        (t.startswith("@") && t.substring(1) is in users => (post(res) = pre(res) U {t.substring(1)}).
    */

    // Restituisce la lista dei post effettuati dall’utente nella 
    // rete sociale il cui nome è dato dal parametro username
    public List<Post> writtenBy(String username)
        throws NullPointerException;
    /*
    REQUIRES:
        username != null.
    THROWS:
        NullPointerException (unchecked) se username vale null.
    MODIFIES:
        ---.
    EFFECTS:
        Itera sui post presenti nel social network: per ciascuno di essi verifica l'uguaglianza tra 
        l'autore e la stringa in input; se è verificata aggiunge il post alla lista in output.
        Formalmente:
        res := lista dei post in output, inizialmente vuota.
        for all p in textualPosts.
        p.getAuthor().equals(username) => post(res) = pre(res) U {p}.
    */

    // Restituisce la lista dei post effettuati dall’utente il cui 
    // nome è dato dal parametro username presenti nella lista ps.
    public static List<Post> writtenBy(List<Post> ps, String username)
        throws NullPointerException
    {
        if (ps == null)
            throw new NullPointerException("List cannot be null.\n");
        if (username == null)
            throw new NullPointerException("Username cannot be null.\n");
        List<Post> res = new ArrayList<>();
        for (Post p : ps)
            if (p == null)
                throw new NullPointerException("List cannot contain a null post.\n");
            else if (p.getAuthor().equals(username) && !SocialNetwork.LikePattern(p.getText()))
                res.add(p);
        return res;
    }
    /*
    REQUIRES:
        ps != null && username != null && for all p in ps. p != null.
    THROWS:
        NullPointerException (unchecked) se ps o username o un elemento della lista ps vale null.
    MODIFIES:
        ---.
    EFFECTS:
        Itera sui post presenti nella lista ps: per ciascuno di essi verifica l'uguaglianza tra 
        l'autore e la stringa in input; se è verificata aggiunge il post alla lista in output.
        Formalmente:
        res := lista dei post in output, inizialmente vuota.
        for all p in ps.
        !ValidLike(p) && p.getAuthor().equals(username) => post(res) = pre(res) U {p}.
    */

    // Restituisce la lista dei post presenti nella rete sociale che includono
    // almeno una delle parole presenti nella lista delle parole argomento del metodo
    public List<Post> containing(List<String> words)
        throws NullPointerException;
    /*
    REQUIRES:
        words != null && for all String w in words. w != null.
    THROWS:
        NullPointerException (unchecked) se words vale null o se esiste un elemento di words che vale null.
    MODIFIES:
        ---.
    EFFECTS:
        Itera sui testi dei post presenti nel social network: per ciascuno di essi verifica la presenza di almeno una 
        delle stringhe presenti in words; se è verificata aggiunge il post alla lista in output.
        Formalmente:
        res := lista dei post in output, inizialmente vuota.
        for all p in textualPosts.
        Exists(w in words | p.getText().contains(w)) => post(res) = pre(res) U {p}.
    */

    // Recupera l'identificativo del post a cui si fa riferimento.
    public static String getReferencedID(String text)
    {
        return text.split("[ ]")[1];
    }
    /*
    REQUIRES:
    ---.
    THROWS:
        ---.
    MODIFIES:
        ---.
    EFFECTS:
        Restituisce l'identificativo del post su cui si vuole operare.
    */

    // Verifica che il parametro in input possa seguire il pattern di un post di tipo like.
    public static boolean LikePattern(String text)
    {
        String LIKEPATTERN = "^Like: \\d+";
        return Pattern.compile(LIKEPATTERN, Pattern.CASE_INSENSITIVE).matcher(text).lookingAt();
    }
    /*
    REQUIRES:
        ---.
    THROWS:
        ---.
    MODIFIES:
        ---.
    EFFECTS:
        Restituisce true sse la stringa text segue il format "^Like: \\d+".
    */

    // Metodo ausiliario usato per controllare se i riferimenti fatti da due Post sono equivalenti.
    public static boolean SameReference(Post p, Post q)
    {
        if (p.getAuthor().equals(q.getAuthor()) && SocialNetwork.getReferencedID(q.getText()).equals(SocialNetwork.getReferencedID(p.getText())))
            return true;
        return false;
    }
    /*
    REQUIRES:
        ---.
    THROWS:
        ---.
    MODIFIES:
        ---.
    EFFECTS:
        Restituisce true sse i due post hanno stesso autore e fanno riferimento allo stesso identificatore.
    */

    // Metodo ausiliario usato per inizializzare gli utenti all'interno delle mappe.
    public static void initializeUser(Map<String, Set<String>> map, String author)
    {
        if (!map.containsKey(author))
            map.put(author, new HashSet<>());
    }
    /*
    REQUIRES:
        ---.
    THROWS:
        ---.
    MODIFIES:
        ---.
    EFFECTS:
        Se la mappa non contiene una chiave uguale a author, le associa l'insieme vuoto.
    */

    // Getter per l'insieme dei post.
    public Set<Post> getPosts();
    /*
    REQUIRES:
        ---.
    THROWS:
        ---.
    MODIFIES:
        ---.
    EFFECTS:
        Restituisce l'insieme dei post testuali presenti nel social network.
    */

    // Getter per l'insieme dei like.
    public Set<Post> getLikes();
    /*
    REQUIRES:
        ---.
    THROWS:
        ---.
    MODIFIES:
        ---.
    EFFECTS:
        Restituisce l'insieme dei post di tipo like presenti nel social network.
    */
}