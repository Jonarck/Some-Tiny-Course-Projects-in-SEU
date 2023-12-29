package drools;

public class Triple {
    private String subject;
    private String predicate;
    private String object;


    public Triple(String subject, String predicate, String object) {
        this.subject = subject;
        this.predicate = predicate;
        this.object = object;


//        this.ovalue = Float.parseFloat(object);
    }

    public static int convert2int(String str) {
        int ovalue;
        // null or empty
        if (str == null || str.length() == 0) {
            return -1;
        }
        try {
            ovalue=Integer.parseInt(str);
            return ovalue;
        }
        catch (NumberFormatException e) {
            return -1;
        }
    }


    public String getSubject() {
        return subject;
    }

    public void setSubject(String subject) {
        this.subject = subject;
    }

    public String getPredicate() {
        return predicate;
    }

    public void setPredicate(String predicate) {
        this.predicate = predicate;
    }

    public String getObject() {
        return object;
    }

    public void setObject(String object) {
        this.object = object;
    }

    @Override
    public String toString() {
        return subject + " " + predicate + " " + object + " .";
    }
}
