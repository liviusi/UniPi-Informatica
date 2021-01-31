import java.util.Set;
import java.util.regex.Pattern;

public interface KiddiesSocialNetwork extends SocialNetwork
{
    /*
    OVERVIEW:
        Interfaccia implementata dalla classe MySocialNetwork.
        Rappresenta una estensione gerarchica del tipo di dato SocialNetwork
        in cui viene implementato un meccanismo di segnalazione per i contenuti
        offensivi all'interno della rete sociale.
        La variabile statica toIgnoreWords rappresenta il contenitore per le stringhe
        che non devono essere considerate durante le operazioni di segnalazione.
        Per altre informazioni sul funzionamento del meccanismo di segnalazione consultare la relazione.

    TYPICAL ELEMENT:
        super.TYPICAL_ELEMENT(),
        {segnalazione1, ..., segnalazionen}.
        
    NOTAZIONE:
        Si estende la notazione ausiliaria.
        - Si definisce Reports := insieme dei post di tipo segnalazione.
        - Si definisce Reported := insieme dei post che sono stati segnalati.
        - Si definisce la funzione ausiliaria SegnalazionePattern(String s) che verifica che il format di una stringa
        sia quello di una segnalazione.
        SegnalazionePattern(s) restituisce true sse s è del tipo "Segnalazione: <int>" (case insensitive).
        - Si definisce la funzione ausiliaria isReportable(String s) che verifica che un post
        possa essere segnalato.
        - Si definisce la funzione ausiliaria ValidReport(Post p) che verifica se un post
        di tipo segnalazione sia valido.
        ValidReport(p) restituisce true sse SegnalazionePattern(p.getText()) && Exists(Post q in TextualPosts | getReferencedID(p.getText()).equals(q.getID())) && isReportable(p.getText()) && 
            !Exists(Post q in Reports | getReferencedID(q.getText()).equals(p.getText()) && q.getAuthor().equals(p.getAuthor())).
    */

    // Getter per l'insieme dei post che non sono mai stati segnalati.
    public Set<Post> getFilteredPosts();
    /*
    REQUIRES:
        ---.
    THROWS:
        ---.
    MODIFIES:
        ---.
    EFFECTS:
        Restituisce l'insieme dei post che non sono stati segnalati.
    */

    // Getter per l'insieme dei post che sono stati segnalati.
    public Set<Post> getReportedPosts();
    /*
    REQUIRES:
        ---.
    THROWS:
        ---.
    MODIFIES:
        ---.
    EFFECTS:
        Restituisce l'insieme dei post che sono stati segnalati.
    */

    // Getter per l'insieme dei post di tipo segnalazione.
    public Set<Post> getSegnalazioni();
    /*
    REQUIRES:
        ---.
    THROWS:
        ---.
    MODIFIES:
        ---.
    EFFECTS:
        Restituisce l'insieme dei post di tipo segnalazione.
    */

    // Verifica che il parametro in input possa seguire il pattern di un post di tipo segnalazione.
    public static boolean SegnalazionePattern(String text)
    {
        String SEGNALAZIONEPATTERN = "^Segnalazione: \\d+";
        return Pattern.compile(SEGNALAZIONEPATTERN, Pattern.CASE_INSENSITIVE).matcher(text).lookingAt();
    }
    /*
    REQUIRES:
        ---.
    THROWS:
        ---.
    MODIFIES:
        ---.
    EFFECTS:
        Restituisce true sse la stringa text segue il format "^Segnalazione: \\d+".
    */

    public static boolean isReportable(String s)
    {
        Pattern CONTAINSLETTERS = Pattern.compile("^[A-Za-z]*");
        String[] tokens = s.split("[ ]");
        if (!KiddiesSocialNetwork.SegnalazionePattern(s) && !SocialNetwork.LikePattern(s))
        {
            for (String t : tokens)
                if (CONTAINSLETTERS.matcher(t).matches())
                    return true;
        }
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
        Restituisce true se s è una stringa segnalabile, ossia se non è composta solo da numeri o da stringhe ignorabili.
    */
}