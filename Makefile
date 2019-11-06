all: server client

server:
	bazel build //:server
	cp bazel-bin/server ./server

client:
	bazel build //:client
	cp bazel-bin/client ./client

clean:
	rm -f server client

hardclean:
	rm -f server client
	bazel clean
