import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;

public class Main {

    public static boolean TestSocialNetwork(SocialNetwork toTest)
    {
        // adding legal posts
        Post p0 = null, p1 = null, p2 = null;
        try
        {
            p0 = toTest.addPost("author0", "Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
            p1 = toTest.addPost("author1", "Maecenas ut augue sed leo posuere sagittis placerat id ipsum.");
            p2 = toTest.addPost("author2", "Suspendisse malesuada ullamcorper interdum.");
        }
        catch (NullPointerException | Post.IllegalPostException e)
        {
            return false;
        }


        // trying out illegal posts
        try
        {
            toTest.addPost("bad-author",
                    "more than 140 characters more than 140 characters more than 140 characters more than 140 characters more than 140 characters more than 140 characters");
            return false;
        }
        catch (NullPointerException | Post.IllegalPostException e)
        {
            System.out.println("Exception successfully caught [line: 33]:\n"  + e + "\n\n");
            assert e instanceof Post.IllegalPostException;
        }
        try
        {
            toTest.addPost("", ""); return false;
        }
        catch (NullPointerException | Post.IllegalPostException e)
        {
            System.out.println("Exception successfully caught [line: 42]:\n"  + e + "\n\n");
            assert e instanceof Post.IllegalPostException;
        }
        try
        {
            toTest.addPost("     ", "text"); return false; // author contains only whitespaces
        }
        catch (NullPointerException | Post.IllegalPostException e)
        {
            System.out.println("Exception successfully caught [line: 51]:\n"  + e + "\n\n");
            assert e instanceof Post.IllegalPostException;
        }
        try
        {
            toTest.addPost("bad-aut     hor", "text"); return false; // author contains whitespaces
        }
        catch (NullPointerException | Post.IllegalPostException e)
        {
            System.out.println("Exception successfully caught [line: 60]:\n"  + e + "\n\n");
            assert e instanceof Post.IllegalPostException;
        }
        try
        {
            toTest.addPost(null, "valid text"); return false;
        }
        catch (NullPointerException | Post.IllegalPostException e)
        {
            System.out.println("Exception successfully caught [line: (69)]!\n"  + e + "\n\n");
            assert e instanceof NullPointerException;
        }
        try
        {
            toTest.addPost("validauthor", null); return false;
        }
        catch (NullPointerException | Post.IllegalPostException e)
        {
            System.out.println("Exception successfully caught [line: 78]:\n"  + e + "\n\n");
            assert e instanceof NullPointerException;
        }

        // trying out legal likes
        try
        {
            toTest.addPost("author4", "Like: " + p0.getID());
            toTest.addPost("author5", "Like: " + p0.getID());
            toTest.addPost("author6", "Like: " + p2.getID());
            toTest.addPost("author7", "Like: " + p2.getID());
            toTest.addPost("author2", "Like: " + p1.getID());
            toTest.addPost("author3", "Like: " + p0.getID());
        }
        catch (NullPointerException | Post.IllegalPostException e)
        {
            return false;
        }

        // trying out illegal likes
        try
        {
            toTest.addPost("author0", "Like: " + p0.getID()); return false;
        }
        catch (NullPointerException | Post.IllegalPostException e)
        {
            System.out.println("Exception successfully caught [line: 104]:\n"  + e + "\n\n");
            assert e instanceof Post.IllegalPostException;
        }
        try
        {
            toTest.addPost("author0", "Like: 1u30914y1"); return false;
        }
        catch (NullPointerException | Post.IllegalPostException e)
        {
            System.out.println("Exception successfully caught [line: 113]:\n"  + e + "\n\n");
            assert e instanceof Post.IllegalPostException;
        }
        try
        {
            toTest.addPost("author0", "Like: " + p0.getID()); return false;
        }
        catch (NullPointerException | Post.IllegalPostException e)
        {
            System.out.println("Exception successfully caught [line: 122]:\n"  + e + "\n\n");
            assert e instanceof Post.IllegalPostException;
        }
        try
        {
            toTest.addPost("author4", "Like: " + p0.getID()); return false;
        }
        catch (NullPointerException | Post.IllegalPostException e)
        {
            System.out.println("Exception successfully caught [line: 131]:\n"  + e + "\n\n");
            assert e instanceof Post.IllegalPostException;
        }
        try
        {
            toTest.addPost("author4", "Like: 42"); return false;
        }
        catch (NullPointerException | Post.IllegalPostException e)
        {
            System.out.println("Exception successfully caught [line: 140]:\n"  + e + "\n\n");
            assert e instanceof Post.IllegalPostException;
        }
                

        // testing guessFollowers, I need a social network to be able to create legal posts
        SocialNetwork ReteSociale2 = new MySocialNetwork();
        Post p3 = null, p4 = null, p5 = null, p6 = null, p7 = null, p8 = null, p9 = null;
        try
        {
            p3 = ReteSociale2.addPost("author1", "this text is meaningless");
            p4 = ReteSociale2.addPost("author7", "like: " + p3.getID());
            p5 = ReteSociale2.addPost("author3", "like: " + p3.getID());
            p6 = ReteSociale2.addPost("author4", "like: " + p3.getID());
            p7 = ReteSociale2.addPost("author5", "this text is meaningless");
            p8 = ReteSociale2.addPost("author3", "like: " + p7.getID());
            p9 = ReteSociale2.addPost("author7", "this text is meaningless");
        }
        catch (NullPointerException | Post.IllegalPostException e)
        {
            return false;
        }
        final Post fp3 = p3, fp4 = p4, fp5 = p5, fp6 = p6, fp7 = p7, fp8 = p8, fp9 = p9;
        List<Post> ps = new ArrayList<>()
        {
            private static final long serialVersionUID = 1L;
            {
                add(fp3);
                add(fp4);
                add(fp5);
                add(fp6);
                add(fp7);
                add(fp8);
                add(fp9);
            }
        };
        // relationships inferred will be:
        // { author1=[], author7=[author1], author5=[], author4=[author1], author3=[author1, author5] }.
        // asserting equality
        assert MySocialNetwork.guessFollowers(ps).equals(new HashMap<>()
        {
            private static final long serialVersionUID = 1L;
            {
                put("author1", new HashSet<>());
                put("author3", new HashSet<>()
                {
                    private static final long serialVersionUID = 1L;
                    {
                        add("author1");
                        add("author5");
                    }
                });
                put("author4", new HashSet<>()
                {
                    private static final long serialVersionUID = 1L;
                    {
                        add("author1");
                    }
                });
                put("author5", new HashSet<>());
                put("author7", new HashSet<>()
                {
                    private static final long serialVersionUID = 1L;
                    {
                        add("author1");
                    }
                });
            }
        });
        try
        {
            MySocialNetwork.guessFollowers(null); return false;
        }
        catch (NullPointerException e)
        {
            System.out.println("Exception successfully caught [line: 215]:\n"  + e + "\n\n");
            assert e instanceof NullPointerException;
        }
        ps.add(null);
        try
        {
            MySocialNetwork.guessFollowers(ps); return false;
        }
        catch (NullPointerException e)
        {
            System.out.println("Exception successfully caught [line: 225]:\n"  + e + "\n\n");
            assert e instanceof NullPointerException;
        }
        ps = null;

        // testing influencers
        // Followers are now:
        // { author2=[author7, author6], author1=[author2], author0=[author5, author4, author3], 
        // author7=[], author6=[], author5=[], author4=[], author3=[] }.
        assert toTest.influencers().toString().equals("[author0, author2, author1, author3, author4, author5, author6, author7]");

        // testing getMentionedUsers
        // will now add some mentions
        Post p10 = null, p11 = null, p12 = null, p13 = null, p14 = null;
        try
        {
            toTest.addPost("author2", "now mentioning @author2");
            toTest.addPost("author4", "now mentioning @author4");
            p14 = toTest.addPost("author1", "now mentioning @author1");
            toTest.addPost("author8", "now mentioning @author3");
            p10 = toTest.addPost("author3", "now mentioning @author2"); // valid
            p11 = toTest.addPost("author5", "now mentioning @author7"); // valid
            p12 = toTest.addPost("author7", "now mentioning @author3"); // valid
            p13 = toTest.addPost("author1", "now mentioning @author5"); // valid
        }
        catch (NullPointerException | Post.IllegalPostException e)
        {
            return false;
        }
        // valid mentions are
        // [author2, author7, author5, author3].
        assert toTest.getMentionedUsers().equals(new HashSet<>()
            {
                private static final long serialVersionUID = 1L;
            {
                add("author2");
                add("author7");
                add("author5");
                add("author3");
            }}
        );
        final Post fp10 = p10, fp11 = p11, fp12 = p12, fp13 = p13;
        assert toTest.getMentionedUsers(new ArrayList<>()
        {
            private static final long serialVersionUID = 1L;
            {
                add(fp10); // valid
                add(fp11); // valid
                add(fp12); // valid
                add(fp13); // valid
            }
        }).equals(new HashSet<>()
        {
            private static final long serialVersionUID = 1L;
            {
                add("author2");
                add("author7");
                add("author5");
                add("author3");
            }
        });
        ps = new ArrayList<>()
        {
            private static final long serialVersionUID = 1L;
            {
                add(fp10);
                add(fp11);
                add(fp12);
                add(null);
            }
        };
        try
        {
            toTest.getMentionedUsers(ps); return false;
        }
        catch (NullPointerException e)
        {
            System.out.println("Exception successfully caught [line: 302]:\n"  + e + "\n\n");
            assert e instanceof NullPointerException;
        }
        ps = null;
        try
        {
            toTest.getMentionedUsers(ps); return false;
        }
        catch (NullPointerException e)
        {
            System.out.println("Exception successfully caught [line: 312]:\n"  + e + "\n\n");
            assert e instanceof NullPointerException;
        }
        
        // testing writtenBy
        assert toTest.writtenBy("").toString().equals("[]");
        final Post fp1 = p1, fp14 = p14;
        ps = new ArrayList<>()
        {
            private static final long serialVersionUID = 1L;
            {
                add(fp1);
                add(fp13);
                add(fp14);
            }
        };
        assert toTest.writtenBy("author1").containsAll(ps) && toTest.writtenBy("author1").size() == ps.size();
        try
        {
            toTest.writtenBy(null); return false;
        }
        catch (NullPointerException e)
        {
            System.out.println("Exception successfully caught [line: 335]:\n"  + e + "\n\n");
            assert e instanceof NullPointerException;
        }
        try
        {
            SocialNetwork.writtenBy(null, "author1"); return false;
        }
        catch (NullPointerException e)
        {
            System.out.println("Exception successfully caught [line: 344]:\n"  + e + "\n\n");
            assert e instanceof NullPointerException;
        }
        try
        {
            SocialNetwork.writtenBy(ps, null); return false;
        }
        catch (NullPointerException e)
        {
            System.out.println("Exception successfully caught [line: 353]:\n"  + e + "\n\n");
            assert e instanceof NullPointerException;
        }
        ps = new ArrayList<>()
        {
            private static final long serialVersionUID = 1L;
            {
                add(fp1);
                add(fp13);
                add(fp14);
            }
        };
        assert SocialNetwork.writtenBy(new ArrayList<>()
        {
            private static final long serialVersionUID = 1L;
            {
                add(fp1);
                add(fp10);
                add(fp11);
                add(fp12);
                add(fp13);
                add(fp14);
            }
        }, "author1").containsAll(ps) && SocialNetwork.writtenBy(new ArrayList<>()
        {
            private static final long serialVersionUID = 1L;
            {
                add(fp1);
                add(fp10);
                add(fp11);
                add(fp12);
                add(fp13);
                add(fp14);
            }
        }, "author1").size() == ps.size();
        try
        {
            SocialNetwork.writtenBy(new ArrayList<>()
            {
                private static final long serialVersionUID = 1L;
                {
                    add(fp1);
                    add(null);
                    add(fp10);
                }
            }, "author1"); return false;
        }
        catch (NullPointerException e)
        {
            System.out.println("Exception successfully caught [line: 402]:\n"  + e + "\n\n");
            assert e instanceof NullPointerException;
        }

        // testing containing
        try
        {
            toTest.containing(null);
        }
        catch (NullPointerException e)
        {
            System.out.println("Exception successfully caught [line: 413]:\n"  + e + "\n\n");
            assert e instanceof NullPointerException;
        }
        try
        {
            toTest.containing(new ArrayList<>()
            {
                private static final long serialVersionUID = 1L;
                {
                    add("NullPointerException");
                    add("incoming");
                    add(null);
                }
            }); return false;
        }
        catch(NullPointerException e)
        {
            System.out.println("Exception successfully caught [line: 430]:\n"  + e + "\n\n");
            assert e instanceof NullPointerException;
        }

        return true;
    }

    public static boolean TestKiddies(KiddiesSocialNetwork toTest)
    {
        // adding legal posts
        Post p0 = null, p1 = null, p2 = null, p3 = null;
        try
        {
            p0 = toTest.addPost("author0", "Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
            p1 = toTest.addPost("author1", "Maecenas ut augue sed leo posuere sagittis placerat id ipsum.");
            p2 = toTest.addPost("author2", "Suspendisse malesuada ullamcorper interdum.");
            p3 = toTest.addPost("author4", "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
        }
        catch (NullPointerException | Post.IllegalPostException e)
        {
            return false;
        }

        Post p4 = null;
        // trying out legal likes
        try
        {
            toTest.addPost("author4", "Like: " + p0.getID());
            toTest.addPost("author5", "Like: " + p0.getID());
            toTest.addPost("author6", "Like: " + p2.getID());
            toTest.addPost("author7", "Like: " + p2.getID());
            toTest.addPost("author2", "Like: " + p1.getID());
            toTest.addPost("author3", "Like: " + p0.getID());
            p4 = toTest.addPost("author0", "Like: " + p3.getID());
        }
        catch (NullPointerException | Post.IllegalPostException e)
        {
            return false;
        }
        
        // trying out legal reports
        Post p5 = null, p6 = null;
        try
        {
            p5 = toTest.addPost("author8", "Segnalazione: " + p0.getID());
            p6 = toTest.addPost("author9", "Segnalazione: " + p1.getID());
        }
        catch (NullPointerException | Post.IllegalPostException e)
        {
            return false;
        }

        // trying out illegal reports
        try
        {
            toTest.addPost("author1", "Segnalazione: " + p1.getID());
        }
        catch (NullPointerException | Post.IllegalPostException e)
        {
            System.out.println("Exception successfully caught [line: 489]:\n" + e + "\n\n");
            assert e instanceof Post.IllegalPostException;
        }
        try
        {
            toTest.addPost("author8", "Segnalazione: " + p0.getID());
        }
        catch (NullPointerException | Post.IllegalPostException e)
        {
            System.out.println("Exception successfully caught [line: 498]:\n" + e + "\n\n");
            assert e instanceof Post.IllegalPostException;
        }
        try
        {
            toTest.addPost("author8", "Segnalazione: " + p4.getID());
        }
        catch (NullPointerException | Post.IllegalPostException e)
        {
            System.out.println("Exception successfully caught [line: 507]:\n" + e + "\n\n");
            assert e instanceof Post.IllegalPostException;
        }
        try
        {
            toTest.addPost("author8", "Segnalazione: 2931u0aaa");
        }
        catch (NullPointerException | Post.IllegalPostException e)
        {
            System.out.println("Exception successfully caught [line: 516]:\n" + e + "\n\n");
            assert e instanceof Post.IllegalPostException;
        }
        try
        {
            toTest.addPost("author8", "Segnalazione: " + p5.getID());
        }
        catch (NullPointerException | Post.IllegalPostException e)
        {
            System.out.println("Exception successfully caught [line: 525]:\n" + e + "\n\n");
            assert e instanceof Post.IllegalPostException;
        }
        Post p7 = null;
        try
        {
            p7 = toTest.addPost("author4", "12334913910!!");
        }
        catch (NullPointerException | Post.IllegalPostException e)
        {
            return false;
        }
        try
        {
            toTest.addPost("author1", "Segnalazione: " + p7.getID());
        }
        catch (NullPointerException | Post.IllegalPostException e)
        {
            System.out.println("Exception successfully caught [line: 543]:\n" + e + "\n\n");
            assert e instanceof Post.IllegalPostException;
        }
        
        // checking getters
        final Post fp5 = p5, fp6 = p6;
        assert toTest.getSegnalazioni().equals(new HashSet<>()
        {
            private static final long serialVersionUID = 1L;
            {
                add(fp5);
                add(fp6);
            }
        });

        // checking whether reports work
        final Post fp0 = p0, fp1 = p1;
        assert toTest.getReportedPosts().equals(new HashSet<>()
        {
            private static final long serialVersionUID = 1L;
            {
                add(fp0);
                add(fp1);
            }
        });

        final Post fp2 = p2, fp3 = p3, fp7 = p7;
        assert toTest.getFilteredPosts().equals(new HashSet<>()
        {
            private static final long serialVersionUID = 1L;
            {
                add(fp2);
                add(fp3);
                add(fp7);
            }
        });

        return true;
    }
    public static void main(String[] args)
    {
        SocialNetwork SN = new MySocialNetwork();
        KiddiesSocialNetwork KSN = new MyKiddiesSocialNetwork();
        if (TestSocialNetwork(SN) && TestSocialNetwork(KSN))
        {
            KSN = new MyKiddiesSocialNetwork();
                if (TestKiddies(KSN))
            {
                System.out.println("SOCIAL NETWORK TESTS PASSED SUCCESSFULLY! ✅\n");
                System.out.println("KIDDIES SOCIAL NETWORK TESTS PASSED SUCCESSFULLY! ✅\n");
            }
        }
    }
}
