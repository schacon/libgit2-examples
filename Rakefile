task :compile do
  `rm -f libgit`
  `gcc -lgit2 -lz -o libgit libgit.c`
  exec "./libgit"
end

task :doc do
  `rocco --language=C --comment-chars="//" libgit.c`
  exec "open libgit.html"
end

task :clean do
  `rm -f libgit`
end
