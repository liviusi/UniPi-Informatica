# PROGRAMMAZIONE II - A.A. 2020 - 21

# Primo Progetto Intermedio

Il progetto ha l’obiettivo di applicare i concetti e le tecniche di programmazione _Object-Oriented_
esaminate durante il corso. Lo scopo del progetto è lo sviluppo di una componente software di
supporto alla gestione e l’analisi di una rete sociale (Social Network) denominata MicroBlog.

La rete sociale consente agli utenti di inviare messaggi di testo di breve lunghezza, con un massimo
di 140 caratteri, chiamati post. Gli utenti possono ‘seguire’ i post di altri utenti.

**Parte 1**

Si richiede di progettare, realizzare e documentare il tipo di dato **_Post_** per rappresentare un post.
Un post è descritto da un insieme di informazioni:

- id: identificatore univoco del post
- author: utente della rete sociale che ha scritto il post
- text: testo (massimo 140 caratteri) del post
- timestamp: data e ora di invio del post

Si definisca la specifica completa del tipo di dato **_Post_** , introducendo i relativi metodi e fornendo le
motivazioni delle scelte effettuate. Possono essere aggiunte altre informazioni motivando
opportunamente la scelta.

Si definisca l’implementazione del tipo di dato Post.

**Parte 2**

Si richiede di progettare, realizzare e documentare il tipo di dato SocialNetwork per operare sulla
rete sociale MicroBlog. Come struttura di implementazione della rete sociale si richiede di utilizzare

**Map<String, Set<String>>**

Intuitivamente, _Map[a]_ definisce l’insieme delle persone seguite nella rete sociale dall’utente _a_. Una
persona sulla rete sociale è rappresentata e identificata in modo dal nome. Gli utenti della rete
sociale non possono seguire se stessi.

Si ipotizzi di avere a disposizione, tra gli altri, i seguenti metodi

1. **public Map<String, Set<String>> guessFollowers(List<Post> ps)** che restituisce la rete
    sociale derivata dalla lista di post (parametro del metodo);
2. **public List<String> influencers(Map<String, Set<String>> followers)** che restituisce gli
    utenti più influenti delle rete sociale (parametro del metodo), ovvero quelli che hanno un
    numero maggiore di “follower”;
3. **public Set<String> getMentionedUsers()** che restituisce l’insieme degli utenti menzionati
    (inclusi) nei post presenti nella rete sociale
4. **public Set<String> getMentionedUsers(List<Post> ps)** che restituisce l’insieme degli utenti
    menzionati (inclusi) nella lista di post;


2

5. **public List<Post> writtenBy(String username)** che restituisce la lista dei post effettuati
    dall’utente nella rete sociale il cui nome è dato dal parametro username
6. **public List<Post> writtenBy(List<Post> ps, String username)** che restituisce la lista dei post
    effettuati dall’utente il cui nome è dato dal parametro username presenti nella lista ps.
7. **public List<Post> containing(List<String> words)** che restituisce la lista dei post presenti
    nella rete sociale che includono almeno una delle parole presenti nella lista delle parole
    argomento del metodo.

Si definisca la specifica completa del tipo di dato SocialNetwork fornendo le motivazioni delle scelte
effettuate. Devono essere aggiunti altri metodi motivando opportunamente la scelta.

Si definisca l’implementazione del tipo di dato Social Network.

**Parte 3**

Si richiede di discutete come sia possibile progettare una estensione gerarchica del tipo di dato
SocialNetwork che permetta di introdurre un criterio per segnalare contenuti offensivi presenti nella
rete sociale. Si presenti la progettazione e realizzazione di almeno una soluzione.

**Nota.**

Per valutare il comportamento di tutte le implementazioni proposte si realizzi una batteria di test.
Le implementazioni proposte devono superare tutte le batteria di test progettate

**Modalità di consegna**

- Il progetto deve essere svolto e discusso col docente individualmente. Il confronto con
    colleghi mirante a valutare soluzioni alternative durante la fase di progetto è incoraggiato.
- Il progetto deve essere costituito da
    o i file sorgente contenenti il codice sviluppato e la batteria di test, ove tutto il codice
       deve essere adeguatamente commentato;
    o una relazione che descrive le principali scelte progettuali ed eventuali istruzioni per
       eseguire il codice.
- La consegna va fatta inviando per email al Prof. Ferrari o alla Prof. ssa Levi con oggetto
    “[PR2A] Consegna Progetto 1” e“[PR2B] Consegna Progetto 1”, rispettivamente.
- Il progetto deve essere consegnato entro il 1 Dicembre 2020.
- **Altre informazioni** Per quanto riguarda il progetto, i docenti risponderanno solo a eventuali
    domande riguardanti l’interpretazione del testo, e non commenteranno soluzioni parziali
    prima della consegna.


