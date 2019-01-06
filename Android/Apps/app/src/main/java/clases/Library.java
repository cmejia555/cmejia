package clases;

public class Library {
    private int id;
    private String bookName;
    private String subject;
    private String details;
    private int imageID;

    public Library() {
        super();
    }

    public Library(int id, String bookName, String subject, String details, int image) {
        this.id = id;
        this.bookName = bookName;
        this.subject = subject;
        this.details = details;
        this.imageID = image;
    }

    public void setId(int id) {
        this.id = id;
    }

    public void setBookName(String bookName) {
        this.bookName = bookName;
    }

    public void setSubject(String subject) {
        this.subject = subject;
    }

    public void setDetails(String details) { this.subject = details;}

    public void setImageID(int imageID) {
        this.imageID = imageID;
    }

    public int getId() {
        return id;
    }

    public String getBookName() {
        return bookName;
    }

    public String getSubject() {
        return subject;
    }

    public String getDetails() {
        return details;
    }

    public int getImageID() {
        return imageID;
    }
}
