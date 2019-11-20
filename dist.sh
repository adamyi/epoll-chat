rm report.pdf
wget -O report.pdf "https://docs.google.com/a/adamyi.com/document/export?format=pdf&id=17CRc4Ejcu5sEyTOdjDHsXUK0wlssklqABTgneGW0BlE"
tar cvf assign.tar report.pdf WORKSPACE BUILD.bazel achelper/BUILD.bazel achelper/*.c achelper/*.h lib/ clientlib/ serverlib/ client.c server.c
give cs3331 assign assign.tar
