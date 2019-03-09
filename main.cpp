
#include "program_utility.hpp"

using namespace cmd_utility;

using namespace std;

void test();

int main (int argn, char **args)
{
    try
    {
        auto program = program_config {size_t( argn ), args}.get_program();
        program();
    }
    catch (invalid_argument const& ex)
    {
        cerr << ex.what();
    }
    catch (file_utility::file_type_exception const& ex)
    {
        cerr << ex.what();
    }
    catch (exception const& ex)
    {
        cerr << "invalid command";
    }
}

void test ()
{
    vector <string> cmd1
    {   "heh"
    ,   "-encode"
    ,   "-i",   "data/tiger.bmp"
    ,   "-o",   "data/tiger_o.bmp"
    ,   "-t",   "lol kek heh loh "
    };

    vector <string> cmd2
    {   "heh"
    ,   "-decode",  "t"
    ,   "-i",       "data/tiger_o.bmp"
    };

    vector <string> cmd3
    {   "heh"
    ,   "-encode"
    ,   "-i",       "data/zmap.bmp"
    ,   "-o",       "data/zmap_en.bmp"
    ,   "-src",     "data/zkars.jpg"
    ,   "-bits",    "1"
    };

    vector <string> cmd4
    {   "heh"
    ,   "-decode",  "src"
    ,   "-i",       "data/zmap_en.bmp"
    ,   "-o",       "data/zkars_de.bmp"
    ,   "-bits",    "1"
    };


    try
    {
        program_config {cmd3.size(), cmd3.begin()}.get_program() ();
        program_config {cmd4.size(), cmd4.begin()}.get_program() ();
    }
    catch (invalid_argument const& ex)
    {
        cerr << ex.what();
    }
    catch (file_utility::file_type_exception const& ex)
    {
        cerr << ex.what();
    }
    catch (exception const& ex)
    {
        cerr << "invalid command";
    }
}
