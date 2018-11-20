/********************************************************************************/ /**
 * @file pico.cpp
 *
 *************************************************************************************/
#include <assert.h>
#include <iostream>
#include <memory>
#include <regex>

#include <picoscm/gc.hpp>
#include <picoscm/parser.hpp>
#include <picoscm/scheme.hpp>

using namespace std;
using namespace pscm;

int main(int argn, char* argv[])
{
    using pscm::Intern, pscm::Cell, pscm::mknum, pscm::mkstr, pscm::nil;

    pscm::Scheme scm;

    scm.mkfun("greet", [cntr = 0](Scheme& scm, const SymenvPtr&, const std::vector<Cell>&) mutable -> Cell {
        return scm.list(mkstr("hello world"), mknum(cntr++));
    });

    if (argn > 1)
        scm.load(argv[1]);
    else
        scm.load("picoscmrc.scm");

    scm.repl();
    return 0;

    try {
        pscm::SymenvPtr env = scm.mkenv();

        // (for-each (lambda (x) x) (quote (1 2 3 4)))
        Cell expr = scm.list(Intern::op_foreach,
            scm.list(Intern::_lambda, scm.list(scm.mksym("x")), scm.mksym("x")),
            scm.list(Intern::_quote, scm.list(pscm::mknum(1), pscm::mknum(2))));

        //        pscm::Parser parser(scm);
        //        std::istringstream stream("(define (hello x)  "
        //                                  "   (display x)     "
        //                                  "   (newline))      "
        //                                  "                   "
        //                                  "(define n 43)      "
        //                                  "(hello n)          ");
        //        Cell expr = parser.read(stream);

        Cell proc = scm.eval(env, expr);
        cout << proc << endl;
        return 0;

    } catch (std::bad_variant_access& e) {
        cout << e.what() << endl;

    } catch (std::invalid_argument& e) {
        cout << e.what() << endl;
    }
    return 0;
}
