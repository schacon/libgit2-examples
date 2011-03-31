task :compile do
  `rm -f libgit`
  `gcc -lgit2 -o libgit libgit.c`
  exec "./libgit"
end

task :doc do
  `rocco --language=C --comment-chars="//" libgit.c`
  `mv libgit.html index.html`
  exec "open index.html"
end

task :publish do
  ENV['GIT_INDEX_FILE'] = '/tmp/i'
  osha = `git hash-object -w index.html`
  `git add -f index.html`
  tsha = `git write-tree`
  csha = `echo 'generated site' | git commit-tree #{tsha}`
  puts "created commit #{csha}"
  `git update-ref refs/heads/gh-pages #{csha}`
  `git push -f origin gh-pages`
end

task :clean do
  `rm -f libgit`
end
