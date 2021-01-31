import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Set;

public class MySocialNetwork implements SocialNetwork
{
    /*
    OVERVIEW:
        Implementazione dell'interfaccia SocialNetwork. La classe MySocialNetwork
        è una collezione mutabile di dati i cui elementi caratteristici sono:
        postsNum := contatore dei post istanziati;
        Follows := mappa (Stringa a -> Insieme di stringhe) definisce l’insieme degli utenti seguiti
        nella rete sociale dall’utente a;
        textualPosts := insieme dei post testuali;
        likePosts := insieme dei post che sono identificabili come likes;
        Followers := mappa (Stringa a -> Insieme di stringhe) definisce l'insieme degli utenti che seguono
        l'utente a all'interno della rete sociale.

    TYPICAL ELEMENT:
        postsNum, 
        {(utente1, {utente_seguito1, ..., utente_seguiton}), ..., 
            (utenten, {utente_seguito1, utente_seguiton})},
        {textualPost1, ..., textualPostn},
        {likePost1, ..., likePostn},
        {(utente1, {utente_che_lo_segue1, ..., utente_che_lo_seguen}), ..., 
            (utenten, {utente_che_lo_segue1, utente_che_lo_seguen})}.

    AF:
        Si ricordano le notazioni ausiliarie introdotte nell'interfaccia.
        Si introducono le seguenti funzioni:
        - g: <User, h(User)> con h(User) sottoinsieme di Users
        t.c. (For all u in h(User). Exists(p in textualPosts | p.getAuthor().equals(u) &&
            Exists(q in likePosts | q.getAuthor().equals(User) && getReferencedID(q.getText()).equals(p.getID()))))
        (i.e. Per ogni elemento u di h(User) esiste un Post di tipo Like pubblicato da User riferito a un 
        TextualPost di u).
        La funzione g(i) abbina a ogni stringa i in Users un insieme di follows.
        - j: <User, k(User)> con k(User) sottoinsieme di Users
        t.c.  For all u in k(User). Exists(p in textualPosts | p.getAuthor().equals(User) &&
            Exists(q in likePosts | q.getAuthor().equals(u) && getReferencedID(q.getText()).equals(p.getID())))
        (i.e. Per ogni elemento u di k(User) esiste un TextualPost di User a cui u ha messo like).
        La funzione j(i) abbina a ogni stringa i in Users un insieme di followers.
        La funzione di astrazione è dunque:
        AF(User) := <g(User), j(User), textualPosts, likePosts>.
    
    IR:
        || Nota al lettore: si consiglia di leggere IR dalla relazione. ||
        Follows != null && textualPosts != null && likePosts != null
        && Followers != null && postsNum = textualPosts.size() + likePosts.size()
        && (for all i in [0; textualPosts.size()]. textualPosts[i] != null)
        && (for all i in [0; likePosts.size()]. likePosts[i] != null)
        && (for all i in [0; Follows.size()]. i != null && Follows[i] != null)
        && (for all (k, v) in Follows.Entry.
        for all u in v. Exists(Post p | textualPosts.cannot contain(p) && p.getAuthor().equals(u) &&
        Exists(Post q | likePosts.cannot contain(q) && q.getAuthor().equals(k) && getReferencedID(q.getText()).equals(p.getID())))
        con Follows.Entry definito come l'insieme delle coppie (key, value) nella map)
        && (for all i in [0; Followers.size()]. i != null &&  Followers[i] != null)
        && (for all (k, v) in Followers.Entry.
        for all u in v. Exists(Post p | textualPosts.cannot contain(p) && p.getAuthor().equals(k) &&
        Exists(q in likePosts | q.getAuthor().equals(u) && getReferencedID(q.getText()).equals(p.getID())))
        con Followers.Entry definito come l'insieme delle coppie (key, value) nella map).
    */

    protected long postsNum; // counts how many posts have been instantiated
    protected Map<String, Set<String>> Follows; // would roughly correspond to Instagram's follows
    private Set<Post> textualPosts; // set corresponding to textual posts
    private Set<Post> likePosts; // set corresponding to like posts
    protected Map<String, Set<String>> Followers; // would roughly correspond to Instagram's followers

    public MySocialNetwork()
    {
        postsNum = 0;
        Follows = new HashMap<>();
        textualPosts = new HashSet<>();
        likePosts = new HashSet<>();
        Followers = new HashMap<>();
    }
    /*
    REQUIRES:
        ---.
    THROWS:
        ---.
    MODIFIES:
        this.
    EFFECTS:
        Inizializza un oggetto della classe MySocialNetwork.
    */

    private static void maintainIR(Map<String, Set<String>> map, String author, String s)
    {
        Set<String> tmp = new HashSet<>();
        if (map.get(s) != null)
            tmp = map.get(s);
        if (!tmp.contains(author))
        {
            tmp.add(author);
            map.put(s, tmp);
        }
    }
    /*
    REQUIRES:
        ---.
    THROWS:
        ---.
    MODIFIES:
        map.
    EFFECTS:
        Mantiene l'invariante di rappresentazione della classe MySocialNetwork verificando le condizioni sulla
        mappa in input: se author non è presente nell'insieme abbinato alla chiave s, lo aggiunge.
    */

    public static Map<String, Set<String>> guessFollowers(List<Post> ps)
        throws NullPointerException
    {
        if (ps == null)
            throw new NullPointerException("List cannot be null.\n");
        Map<String, Set<String>> res = new HashMap<>();
        Map<String, String> tmp = new HashMap<>();
        for (Post p : ps)
        {
            if (p == null) throw new NullPointerException("List cannot contain a null post.\n");
            if (!SocialNetwork.LikePattern(p.getText())) tmp.put(p.getID(), p.getAuthor());
            if (!res.containsKey(p.getAuthor())) res.put(p.getAuthor(), new HashSet<>());
        }
        // tmp adesso contiene tutti i post testuali
        for (Post p : ps) {
            if (SocialNetwork.LikePattern(p.getText()))
            {
                // System.out.println("LIKE ID: " + getReferencedID(p.getText()));
                if (tmp.keySet().contains(SocialNetwork.getReferencedID(p.getText())))
                    maintainIR(res, tmp.get(SocialNetwork.getReferencedID(p.getText())), p.getAuthor());
            }
        }
        return res;
    }
    /*
    REQUIRES:
        ps != null && for all p in ps. p != null.
    THROWS:
        NullPointerException (unchecked) se ps vale null o se esiste un post in ps che vale null.
    MODIFIES:
        ---.
    EFFECTS:
        Itera sulla lista di post passata in input: distingue i post non di tipo like dai post di tipo like.
        Aggiunge alla rete sociale gli autori di tutti i post e, se questi sono di tipo like, aggiorna la rete sociale
        in output mantenendone l'invariante di rappresentazione.
        Formalmente:
        res := mappa da stringhe (utenti) in insieme di stringhe (followers) in output.
        a := post non di tipo like con identificativo getReferencedID(p.getText()).
        for all p in ps.
        !ValidLikePost(p) => post(res) = pre(res) U (p.getAuthor(), {})
            && ValidLikePost(p) => post(res[p.getAuthor()]) = pre(res[p.getAuthor()]) U {a.getAuthor()}
    */

    private Post createLike(Post p)
        throws Post.IllegalPostException
    {
        String ID = SocialNetwork.getReferencedID(p.getText());
        for (Post q : textualPosts)
        {
            if (q.getID().equals(ID))
            {
                
                if (q.getAuthor().equals(p.getAuthor()))
                    throw new Post.IllegalPostException(p.getAuthor() + " cannot like a post of theirs.\n");
                if (!Follows.isEmpty() && Follows.get(p.getAuthor()) != null && Follows.get(p.getAuthor()).contains(q.getAuthor()))
                {
                    for (Post r : likePosts)
                        if (SocialNetwork.SameReference(p, r))
                            throw new Post.IllegalPostException("Author has already liked this post. [Existing Like ID: " + r.getID() + "]\n");
                }
                if (likePosts.add(p))
                {
                    SocialNetwork.initializeUser(Follows, p.getAuthor());
                    SocialNetwork.initializeUser(Followers, p.getAuthor());
                    maintainIR(Follows, q.getAuthor(), p.getAuthor());
                    maintainIR(Followers, p.getAuthor(), q.getAuthor());
                    postsNum++;
                    return p;
                }
            }
        }
        throw new Post.IllegalPostException("Post to like could not be found. [ID: " + p.getID() + "]\n");
    }
    /*
    REQUIRES:
        ValidLike(p).
    THROWS:
        IllegalPostException (checked) se non vale ValidLike(p).
    MODIFIES:
        this.
    EFFECTS:
        Aggiunge un post di tipo like alla rete sociale.
        Formalmente:
        a := post non di tipo like con identificativo getReferencedID(p.getText()).
        post(likePosts) = pre(likePosts) U {p}.
        follows[p.getAuthor()] := insieme degli utenti seguiti dall'autore del post p.
        followers[a.getAuthor()] :=  insieme degli utenti che seguono l'autore del post a.
        post(follows[p.getAuthor()]) = pre(follows[p.getAuthor()]) U {a.getAuthor()}.
        post(followers[a.getAuthor()]) = pre(followers[a.getAuthor()]) U {p.getAuthor()}.
        post(followers[p.getAuthor()]) = pre(followers[p.getAuthor()]) U {}.
    */

    public Post addPost(String author, String text)
        throws NullPointerException, Post.IllegalPostException
    {
        Post p = null;
        try
        {
            p = new MyPost(postsNum, author, text);
        }
        catch (NullPointerException | Post.IllegalPostException e)
        {
            throw e;
        }
        if (SocialNetwork.LikePattern(text))
        {
            try
            {
                return createLike(p);
            }
            catch (Post.IllegalPostException e)
            {
                throw e;
            }
        }
        if (textualPosts.add(p))
        {
            postsNum++;
            SocialNetwork.initializeUser(Follows, p.getAuthor());
            SocialNetwork.initializeUser(Followers, p.getAuthor());
        }
        return p;
    }

    public List<String> influencers()
    {
        List<Entry<String, Set<String>>> list = new LinkedList<>(Followers.entrySet());
        Collections.sort(list, new Comparator<Entry<String, Set<String>>>()
        {
            public int compare(Entry<String, Set<String>> e1, Entry<String, Set<String>> e2)
            { return Integer.compare(e1.getValue().size(), e2.getValue().size()); }
        });
        List<String> res = new LinkedList<>();
        for (Entry<String, Set<String>> l: list) res.add(l.getKey());
        Collections.reverse(res); // now it is in a descending order
        return res;
    }

    // Restituisce l'insieme di sottostringhe di s che iniziano per "@" e che identificano un utente all'interno di Follows.
    private Set<String> detectMention(Post p)
        throws NullPointerException
    {
        if (p == null)
            throw new NullPointerException("Post cannot be null.\n");
        Set<String> res = new HashSet<>();
        String delims = "[ ]";
        String[] tokens = p.getText().split(delims);
        for (String t : tokens)
            if (t.startsWith("@")) 
                if (!(p.getAuthor().equals(t.substring(1))) && Follows.containsKey(t.substring(1)))
                    res.add(t.substring(1));
        return res;
    }
    /*
    REQUIRES:
        p != null.
    THROWS:
        NullPointerException (unchecked) se p vale null.
    MODIFIES:
        ---.
    EFFECTS:
        Itera sul testo di un post e lo separa in sottostringhe delimitate da spazi bianchi. Se una di questa comincia per "@"
        e corrisponde al nome utente di un utente presente in Follows, lo aggiunge all'insieme degli utenti menzionati.
        Formalmente:
        res := insieme di stringhe in output, inizialmente definito come insieme vuoto.
        tokens := s.split("[ ]").
        for t in tokens.
        (t.startswith("@") && t.substring(1) is in Users) => (post(res) = pre(res) U (t.substring(1))).
    */

    public Set<String> getMentionedUsers()
    {
        Set<String> res = new HashSet<>();
        for (Post p : textualPosts)
            res.addAll(detectMention(p));
        return res;
    }

    public Set<String> getMentionedUsers(List<Post> ps)
        throws NullPointerException
    {
        if (ps == null)
            throw new NullPointerException("List cannot be null.\n");
        Set<String> res = new HashSet<>();
        for (Post p : ps) 
            if (p == null)
                throw new NullPointerException("List cannot contain a null post.\n");
            else res.addAll(detectMention(p));
        return res;
    }

    public List<Post> writtenBy(String username)
        throws NullPointerException
    {
        if (username == null)
            throw new NullPointerException("Username cannot be null.\n");
        List<Post> res = new LinkedList<>();
        for (Post p : textualPosts)
            if (p.getAuthor().equals(username))
                res.add(p);
        return res;
    }

    public List<Post> containing(List<String> words)
        throws NullPointerException 
    {
        if (words == null)
            throw new NullPointerException("List cannot be null.\n");
        List<Post> res = new ArrayList<>();
        for(Post p : textualPosts)
        {
            for (String w : words)
                if (w == null)
                    throw new NullPointerException("List cannot contain a null string.\n");
                else if(p.getText().toLowerCase().contains(w.toLowerCase()))
                {
                    res.add(p);
                    break;
                }
        }
        return res;
    }

    public Set<Post> getPosts() { return new HashSet<>(textualPosts); }

    public Set<Post> getLikes() { return new HashSet<>(likePosts); }
}