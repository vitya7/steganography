
#include "program_utility.hpp"

using namespace cmd_utility;

using namespace std;

int main (int argn, char **args)
{
    try
    {
        auto program = program_config {argn, args}.get_program();
        program();
    }
    catch (invalid_argument const& ex)
    {
        cerr << ex.what();
    }
    catch (exception const& ex)
    {
        cerr << "invalid command";
    }
}
