./scummtr2po --export --test --strings test/strings.txt --po test/atlantisca.po
./scummtr2po --import --config test/strings.txt.cfg -s test/stringsBack.txt -p test/atlantisca.po 
diff test/strings.txt test/stringsBack.txt
