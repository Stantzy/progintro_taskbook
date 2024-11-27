#include <stdio.h>
#include <locale.h>
#include <libintl.h>

#define LOCALEBASEDIR "."
#define TEXTDOMAIN "hello_bye"

#define _(STR) gettext(STR)
#define N_(STR) (STR)

int main()
{
	setlocale(LC_CTYPE, "");
	setlocale(LC_MESSAGES, "");
	bindtextdomain(TEXTDOMAIN, LOCALEBASEDIR);
	textdomain(TEXTDOMAIN);

	printf(_("Hello, world!\n"));
	printf(_("We are speaking.\n"));
	printf(N_("This string will not be translated.\n"));
	printf(_("Goodbye, world!\n"));
	return 0;
}
