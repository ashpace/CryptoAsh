//Welcome to the CryptoAsh main source code
//written by Ashkan Rajabzadeh Daghagh
//Date: 18/01/2015
//Project Name:C++ Cryptography

#include <FL/Fl.H>
#include <FL/Fl_Window.h>
#include <FL/Fl_Box.h>
#include <FL/Fl_Button.H>
#include <FL/Fl_File_Input.H>
#include <FL/Fl_Secret_Input.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <iostream>
#include <string>
#include <istream>
#include <ostream>
#include <fstream>
#include <cmath>

// FLTK widgets defined
Fl_Window *DEc;
Fl_Window *ENc;
Fl_Window *welcome;
Fl_Window *Compile_Win;
Fl_Window *waiting_Win;
Fl_Window *Txt_Win;
Fl_Box *box;
Fl_Box *box_Error;
Fl_Box *box_Error1;
Fl_Box *box_Waiting;
Fl_Box *box_Compile;
Fl_Box *box_Compile_address;
Fl_Button *Encryption;
Fl_Button *Decryption;
Fl_File_Input *ENcFile;
Fl_File_Input *DEcFile;
Fl_File_Input *Actual_File;
Fl_File_Input *ENc_File;
Fl_Button *Decryption_Compile;
Fl_Button *Encryption_Compile;
Fl_Button *Decryption_back;
Fl_Button *Encryption_back;
Fl_Button *Exit;
Fl_Button *close_error_but;
Fl_Button *Browse_ENc_In_Dec;
Fl_Button *Browse_ACt_In_ENc;
Fl_Button *Browse_ENc_In_ENc;
Fl_Button *Browse_DEc_In_Dec;
Fl_Button *Compile_Success_Exit;
Fl_Button *Compile_Success_Welcome;
Fl_Button *Compile_Success_view;
Fl_Button *Txt_File_Hide;
Fl_Window *Error_Win;
Fl_Secret_Input *DEc_Pass;
Fl_Secret_Input *ENc_Pass;
Fl_Secret_Input *ENc_Pass_Check;
Fl_Text_Display    *Txt_File;
Fl_Text_Buffer    *Txt_File_Buff;
const char *Result_Address;                //This is the address of the destination file of the compiler. Globally defined to connect information between functions

using namespace std;

//functions propotype
void Error_show(const char* txt);                                        //show an error window showing txt input to the function
const char* file_chooser();                                                //opens a browser to selct file & returns address of the selected file
int IntPass( const char* password, int size);                            //converts a "const char" password into a long integer password
int Encryption_function (char TheCharcter, int position, int IntPass);    //decrypts a single char using its position and the integer pasword
char Decryption_Function (int TheNumber, int position, int IntPass);    //encrypts a single char using its position and the integer pasword
void Compile_Message(const char* file_address, const char* message );    //shows a compiled message using the file address and the desired message
void View_File(const char* file_address);                                //view the plain txt file with the address provided to it
//calback functions
void Compile_decryption(Fl_Widget* w)                  //Decryption compiling function
{
    ifstream Encrypted;                          //defining the files in the function
    ofstream Decrypted;
    //opening the Encryption and Decryption files & checking that they are opened properly
    Encrypted.open ( ENcFile->value(), std::ifstream::in);
    if (Encrypted.is_open())
    {
        Decrypted.open ( DEcFile->value(), std::ofstream::out);
        if (Decrypted.is_open())
        {
            //The decryption proccess starts from here
            DEc->hide();                            //Closing the compile page
            waiting_Win->show();                    //prompting "Please Wait"
            int i = 0, c;
            char e;
            while (Encrypted >> c)                   // loop getting single characters
            {
                i++;
                e = Decryption_Function (c, i, IntPass(DEc_Pass->value(), DEc_Pass->size()));    //Each character is decrypted seperatly
                Decrypted.put(e) ;
            }
            Encrypted.close();
            Decrypted.close();                        // close files
            waiting_Win->hide();
            Compile_Message(DEcFile->value(), "Decryption was successful. The Decryption file is saved at the address below" );
        }
        else Error_show("The input decrypted file is not valid");
    }
    else  Error_show("The input encrypted file is not valid");
}
void Compile_encryption(Fl_Widget* w)
{
    ifstream Actual;                            //defining the files in the function
    ofstream Encrypted;
    Actual.open ( Actual_File->value(), std::ifstream::in);
    if (Actual.is_open())
    {
        Encrypted.open ( ENc_File->value(), std::ofstream::out);
        if (Encrypted.is_open())
        {
            if (IntPass(ENc_Pass->value(), ENc_Pass->size()) == IntPass(ENc_Pass_Check->value(), ENc_Pass_Check->size()))
            {
                ENc->hide();
                waiting_Win->show();
                int i = 0, e;
                char c;
                while (Actual.get(c))           // loop getting single characters
                {
                    i++;
                    e = Encryption_function (c, i, IntPass(ENc_Pass->value(), ENc_Pass->size())); //Each char is encrypted seperatly
                    Encrypted << e << endl ;
                }
                Encrypted.close();
                Actual.close();                // close file
                waiting_Win->hide();
                Compile_Message(ENc_File->value(), "Encryption was successful. The Encrypted file is saved at the address below" );
            }
            else Error_show("Passwords do not match");
        }
        else Error_show("The input encryption file is not valid");
    }
    else  Error_show("The input file is not valid");
}
void Browser_ENc_In_Dec_CB(Fl_Widget*, void*)
{
    ENcFile->value(file_chooser());
}
void Browser_DEc_In_Dec_CB(Fl_Widget*, void*)
{
    DEcFile->value(file_chooser());
}
void Browser_ACt_In_ENc_CB(Fl_Widget*, void*)
{
    Actual_File->value(file_chooser());
}
void Browser_ENc_In_ENc_CB(Fl_Widget*, void*)
{
    ENc_File->value(file_chooser());
}
void show_decryption(Fl_Widget* w)
{
    DEc->show();
    welcome->hide();
}
void show_encryption(Fl_Widget* w)
{
    ENc->show();
    welcome->hide();
}
void Back_decryption(Fl_Widget* w)
{
    DEc->hide();
    welcome->show();
}
void Back_encryption(Fl_Widget* w)
{
    ENc->hide();
    welcome->show();
}
void close_error (Fl_Widget* w)
{
    Error_Win->hide();
}
void Exit_Calback(Fl_Widget* widget, void*)
{
    welcome->hide();
}
void Compiler_Success_Welcome(Fl_Widget*, void*)
{
    Compile_Win->hide();
    welcome->show();
}
void Compiler_Success_Exit(Fl_Widget*, void*)
{
    Compile_Win->hide();
}
void Compiler_Success_View(Fl_Widget*, void*)
{
    View_File(Result_Address);
}
void Txt_File_Hide_CB(Fl_Widget*, void*)
{
    Txt_Win->hide();
}
int main ()
{
    //Welcome window
    welcome = new Fl_Window(400, 100, "Welcome to CryptoAsh");                     
    welcome->begin();
    box = new Fl_Box(10, 10, 380, 30, "Please select your desired fuction");
    Encryption = new Fl_Button(20, 55, 100, 35, "Encryption");
    Encryption->labelfont(FL_HELVETICA_BOLD);
    Encryption->callback(show_encryption);
    Decryption = new Fl_Button(280, 55, 100, 35, "Decryption");
    Decryption->labelfont(FL_HELVETICA_BOLD);
    Decryption->callback(show_decryption);
    Exit = new Fl_Button(150, 55, 100, 35, "Exit program");
    Exit->callback(Exit_Calback);
    welcome->end();
    welcome->show();
   
    //Decryption window (DEc)
    DEc = new Fl_Window(500, 400, "DecryptoAsh");
    DEc->begin();
    ENcFile = new Fl_File_Input(240, 10, 240, 50, "Path of the encrypted file");
    Browse_ENc_In_Dec = new Fl_Button(385, 70, 100, 30, "Browse");
    Browse_ENc_In_Dec->callback(Browser_ENc_In_Dec_CB);
    DEcFile = new Fl_File_Input(240, 120, 240, 50, "Desired path of the decrypted file");
    Browse_DEc_In_Dec = new Fl_Button(385, 180, 100, 30, "Browse");
    Browse_DEc_In_Dec->callback(Browser_DEc_In_Dec_CB);
    DEc_Pass = new Fl_Secret_Input (240, 220, 240, 25, "Password of Decryption");
    Decryption_Compile = new Fl_Button(385, 355, 100, 35, "Compile");
    Decryption_Compile->callback(Compile_decryption);
    Decryption_back = new Fl_Button(275, 355, 100, 35, "Back");
    Decryption_back->callback(Back_decryption);
    DEc->end();
   
    //Encryption window (DEc)
    ENc = new Fl_Window(500, 400, "EncryptoAsh");
    ENc->begin();
    Actual_File = new Fl_File_Input(240, 10, 240, 50, "Path of the  file");
    Browse_ACt_In_ENc = new Fl_Button(385, 70, 100, 30, "Browse");
    Browse_ACt_In_ENc->callback(Browser_ACt_In_ENc_CB);
    ENc_File = new Fl_File_Input(240, 120, 240, 50, "Desired path of the encrypted file");
    Browse_ENc_In_ENc = new Fl_Button(385, 180, 100, 30, "Browse");
    Browse_ENc_In_ENc->callback(Browser_ENc_In_ENc_CB);
    ENc_Pass = new Fl_Secret_Input (240, 220, 240, 25, "Password of Encryption");
    ENc_Pass_Check = new Fl_Secret_Input (240, 255, 240, 25, "Verify Password");
    Encryption_Compile = new Fl_Button(385, 355, 100, 35, "Compile");
    Encryption_Compile->callback(Compile_encryption);
    Encryption_back = new Fl_Button(275, 355, 100, 35, "Back");
    Encryption_back->callback(Back_encryption);
    ENc->end();
   
    //Waiting window
    waiting_Win = new Fl_Window(250, 60, "Compiling in progress");
    waiting_Win->begin();
    box_Waiting = new Fl_Box(5, 5, 200, 55, "Please Waite");
    box_Waiting->labelfont(FL_HELVETICA_BOLD);
    box_Waiting->labelsize(32);
    waiting_Win->end();
    return Fl::run();
}
//Functions defined
void Error_show(const char* txt)
{
    //Error window
    Error_Win = new Fl_Window(400, 100, "Error in EncryptoAsh");
    Error_Win->begin();
    box_Error = new Fl_Box(10, 10, 380, 30, txt);
    box_Error->labelfont(FL_HELVETICA_BOLD);
    close_error_but = new Fl_Button(290, 50, 100, 35, "close");
    close_error_but->callback(close_error);
    box_Error1 = new Fl_Box(10, 50, 100, 40, "Error !");
    box_Error1->labelfont(FL_HELVETICA_BOLD);
    box_Error1->labelsize(30);
    Error_Win->end();
    Error_Win->show();
}
const char* file_chooser()
{
    // Create the file chooser, and show it
    Fl_File_Chooser chooser(".",                        // directory
                            "Txt Files (*.txt)",        // filter
                            Fl_File_Chooser::CREATE,    // chooser type
                            "CryptoAsh File Browser");  // title
    chooser.show();
    // Block until user picks something.
    //     (The other way to do this is to use a callback())
    //
    while (chooser.shown())
    {
        Fl::wait();
    }
    return chooser.value();
}
int IntPass( const char* password, int size)
{
    int IntPass = 0;
    int i;
    for ( i = 0; i < size ; i++)
    {
        IntPass = IntPass + (1000 ^ (i)) * (int)password[i];
    }
}
int Encryption_function (char TheCharcter, int position, int IntPass)
{
    //Encryption algorism
    int C = TheCharcter; //converting the character into ASCII number
    int i = 0;
    C = C + (IntPass * (IntPass % position));
    return C;
}
char Decryption_Function (int TheNumber, int position, int IntPass)
{
    TheNumber = TheNumber - (IntPass * (IntPass % position));
    char Replacement = TheNumber;
    return Replacement;
}
void Compile_Message(const char* file_address, const char* message )
{
    Result_Address = file_address;
    //Compile window
    Compile_Win = new Fl_Window(800, 160, "Sucess");
    Compile_Win->begin();
    box_Compile = new Fl_Box(10, 10, 780, 30, message);
    box_Compile->labelfont(FL_HELVETICA_BOLD);
    box_Compile_address = new Fl_Box(10, 50, 780, 30, file_address);
    Compile_Success_view = new Fl_Button(300, 90, 200, 20, "View the file");
    Compile_Success_Exit = new Fl_Button(410, 120, 100, 35, "Exit");
    Compile_Success_Welcome = new Fl_Button(290, 120, 100, 35, "Main Menu");
    Compile_Success_Welcome->callback(Compiler_Success_Welcome);
    Compile_Success_Exit->callback(Compiler_Success_Exit);
    Compile_Success_view->callback(Compiler_Success_View);
    Compile_Win->end();
    Compile_Win->show();
}
void View_File(const char* file_address)
{
    Txt_Win = new Fl_Window (600, 500, "CryptoAsh File Viewer");
    Txt_Win->begin();
    Txt_File = new Fl_Text_Display(25, 25, 550, 450, "Click on 'Hide' to hide the window");
    Txt_File_Buff = new Fl_Text_Buffer();
    Txt_File->buffer(Txt_File_Buff);
    Txt_File_Buff->loadfile(file_address);
    int pos = 0;
    while (Txt_File_Buff->findchar_forward(pos, '\r', &pos))
    {
        Txt_File_Buff->remove(pos, pos + 1);
    }
    Txt_File_Hide = new Fl_Button(200, 475, 100, 25, "Hide");
    Txt_File_Hide->callback(Txt_File_Hide_CB);
    Txt_Win->end();
    Txt_Win->show();
}

