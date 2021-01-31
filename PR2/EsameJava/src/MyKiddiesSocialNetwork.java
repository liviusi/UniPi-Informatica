import java.util.HashSet;
import java.util.Set;


public class MyKiddiesSocialNetwork extends MySocialNetwork implements KiddiesSocialNetwork
{

    /*
    OVERVIEW:
        Estensione della classe MySocialNetwork che implementa l'interfaccia KiddiesSocialNetwork.
        MyKiddiesSocialNetwork rappresenta un Social Network che implementa un meccanismo di segnalazione,
        è una collezione mutabile di dati i cui elementi caratteristici sono:
        reportPosts := insieme dei post di tipo segnalazione.

    TYPICAL ELEMENT:
        SocialNetwork.TYPICAL_ELEMENT(),
        {segnalazione1, ..., segnalazionen}.
    AF:
        <g(User), j(User), textualPosts, likePosts, reportPosts, Reported>.
    IR:
        super.IR() &&
        reportPosts != null && for all p in reportPosts. p != null.
    */

    private Set<Post> reportPosts;

    // Costruttore di default per la classe MyKiddiesSocialNetwork.
    public MyKiddiesSocialNetwork()
    {
        super();
        reportPosts = new HashSet<>();
    }
    /*
    REQUIRES:
        ---.
    THROWS:
        ---.
    MODIFIES:
        this.
    EFFECTS:
        Inizializza un oggetto della classe MyKiddiesSocialNetwork.
    */

    @Override
    public Post addPost(String author, String text)
        throws NullPointerException, Post.IllegalPostException
    {
        if (KiddiesSocialNetwork.SegnalazionePattern(text))
        {
            try
            {
                return createReport(author, text);
            }
            catch (Post.IllegalPostException e)
            {
                throw e;
            }
        }
        return super.addPost(author, text);
    }
    /*
    REQUIRES:
        super.requires() && SegnalazionePattern(text) => ValidSegnalazione(p).
    THROWS:
        super.throws(),
        IllegalPostException se vale SegnalazionePattern(text) ma non ValidSegnalazione(p).
    MODIFIES:
        super.modifies().
    EFFECTS:
        Istanzia un post coi parametri in input e lo inserisce nell'insieme dei post presenti nel social network.
        Formalmente:
        Si ricorda la notazione aggiuntiva introdotta nelle postcondizioni del metodo addPost definite nell'interfaccia SocialNetwork.
        (p is not in TextualPosts && !ValidLike(p) && !ValidSegnalazione(p) => 
            post(TextualPosts) = pre(TextualPosts) U {p})
        && (p is not in Likes && ValidLike(p) =>
            post(follows[p.getAuthor()]) = pre(follows[p.getAuthor()]) U {a.getAuthor()} &&
            post(followers[a.getAuthor()]) = pre(followers[a.getAuthor()]) U {p.getAuthor()} &&
            post(likes) = pre(likes) U {p})
        && (p is not in Reports && ValidSegnalazione(p) => 
            post(Reports) = pre(Reports) U {p} &&
            post(Reported) = pre(Reported) U {Post q | q.getID().equals(getReferencedID(text))}.
    */

    private Post createReport(String author, String text)
        throws Post.IllegalPostException
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
        for (Post q : getPosts())
        {
            if (q.getID().equals(SocialNetwork.getReferencedID(text)))
            {
                if (q.getAuthor().equals(author))
                    throw new Post.IllegalPostException(author + " cannot report a post of theirs.\n");
                else
                {
                    if (!KiddiesSocialNetwork.isReportable(q.getText()))
                        throw new Post.IllegalPostException("Post with text " + q.getText() + " cannot be reported.\n");
                    else
                    {
                        for (Post r : reportPosts)
                        {
                            if (SocialNetwork.SameReference(r, p))
                                throw new Post.IllegalPostException(author + " has already reported this post. [Existing Report ID: " + r.getID() + "]\n");
                        }
                        reportPosts.add(p);
                        SocialNetwork.initializeUser(Follows, author);
                        SocialNetwork.initializeUser(Followers, author);
                        postsNum++;
                        return p;
                    }
                }
            }
        }
        throw new Post.IllegalPostException("Post to report could not be found. [ID: " + SocialNetwork.getReferencedID(text) + "]\n");
    }
    /*
    REQUIRES:
        Post.requires() && ValidSegnalazione(p),
        con p := oggetto di tipo Post | p.getAuthor().equals(author) && p.getText().equals(text).
    THROWS:
        Post.throws(),
        IllegalPostException (checked) se non vale ValidReport(p).
    MODIFIES:
        this.
    EFFECTS:
        np := oggetto di tipo Post | np.getAuthor().equals(author) && np.getText().equals("Segnalazione: " + p.getID())
        p is in TextualPosts => post(Reports) = pre(Reports) U {np}
        && post(Reported) = pre(Reported) U {p}.
    */

    public Set<Post> getSegnalazioni() { return new HashSet<>(reportPosts); }

    public Set<Post> getReportedPosts()
    {
        Set<Post> src = new HashSet<>(getPosts());
        Set<Post> res = new HashSet<>();
        for (Post p : getSegnalazioni())
        {
            for (Post q : src)
                if (SocialNetwork.getReferencedID(p.getText()).equals(q.getID()))
                {
                    src.remove(q);
                    res.add(q);
                    break;
                }
        }
        return res;
    }
    
    public Set<Post> getFilteredPosts()
    {
        Set<Post> res = new HashSet<>(getPosts());
        res.removeAll(getReportedPosts());
        return res;
    }
    
}
