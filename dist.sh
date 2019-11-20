rm proto/*.c proto/*.h
bazel build //achelper:ac_protoc
bazel-bin/achelper/ac_protoc request.proto response.proto proto
rm report.pdf
wget -O report.pdf "https://docs.google.com/a/adamyi.com/document/export?format=pdf&id=17CRc4Ejcu5sEyTOdjDHsXUK0wlssklqABTgneGW0BlE"
tar cvf assign.tar report.pdf WORKSPACE BUILD.bazel Makefile achelper/BUILD.bazel achelper/*.c achelper/*.h lib/ clientlib/ serverlib/ proto/ client.c server.c *.proto
give cs3331 assign assign.tar
