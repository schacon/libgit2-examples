#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <git2.h>

int main (int argc, char** argv)
{
  git_repository *repo;
  git_repository_open(&repo, "/opt/libgit2-test/.git");
  /* ---------------------------- */

  printf("*Hex to Raw*\n");
  char hex[] = "fd6e612585290339ea8bf39c692a7ff6a29cb7c3";
  git_oid oid;
  git_oid_mkstr(&oid, hex);
  printf("Raw 20 bytes: [%s]\n", (&oid)->id);

  /* ---------------------------- */

  printf("\n*Raw to Hex*\n");
  char out[41];
  out[41] = 0;
  git_oid_fmt(out, &oid);
  printf("SHA hex string: %s\n", out);

  /* ---------------------------- */

  printf("\n*Raw Object Read*\n");
  git_odb *odb;
  git_odb_object *obj;
  git_otype otype;

  unsigned char *data;
  const char *str_type;
  int error;

  odb = git_repository_database(repo);

  error = git_odb_read(&obj, odb, &oid);

  data = (char*)git_odb_object_data(obj);
  otype = git_odb_object_type(obj);

  str_type = git_object_type2string(otype);
  printf("object length and type: %d, %s\n", (int)git_odb_object_size(obj), str_type);

  git_odb_object_close(obj); // close the object when you are done with it

  /* ---------------------------- */

  printf("\n*Raw Object Write*\n");

  git_oid oid2;
  git_odb_write(&oid2, odb, "test data", 9, GIT_OBJ_BLOB);
  git_oid_fmt(out, &oid2);
  printf("Written Object: %s\n", out);

  /* ---------------------------- */

  printf("\n*Commit Parsing*\n");

  git_commit *commit;
  git_oid oid3;

  git_oid_mkstr(&oid3, "f0877d0b841d75172ec404fc9370173dfffc20d1");

  error = git_commit_lookup(&commit, repo, &oid3);

  const git_signature *author, *cmtter;
  const char *message, *message_short;
  time_t ctime;
  unsigned int parents, p;

  message  = git_commit_message(commit);
  message_short = git_commit_message_short(commit);
  author   = git_commit_author(commit);
  cmtter   = git_commit_committer(commit);
  ctime    = git_commit_time(commit);

  printf("Author: %s (%s)\n", author->name, author->email);

  parents  = git_commit_parentcount(commit);
  for (p = 0;p < parents;p++) {
    git_commit *parent;
    git_commit_parent(&parent, commit, p);
    git_oid_fmt(out, git_commit_id(parent));
    printf("Parent: %s\n", out);
    git_commit_close(parent);
  }

  git_commit_close(commit);

  /* ---------------------------- */

  printf("\n*Commit Writing*\n");
  git_oid tree_id, parent_id, commit_id;

  /* create signatures */
  author = git_signature_new("Scott Chacon", "schacon@gmail.com", 123456789, 60);
  cmtter = git_signature_new("Scott A Chacon", "scott@github.com", 987654321, 90);

  git_oid_mkstr(&tree_id, "28873d96b4e8f4e33ea30f4c682fd325f7ba56ac");
  git_oid_mkstr(&parent_id, "f0877d0b841d75172ec404fc9370173dfffc20d1");

  git_commit_create_v(
    &commit_id, /* out id */
    repo,
    NULL, /* do not update the HEAD */
    author,
    cmtter,
    "example commit",
    &tree_id,
    1, &parent_id);

  git_oid_fmt(out, &commit_id);
  printf("New Commit: %s\n", out);

  /* ---------------------------- */

  printf("\n*Tag Parsing*\n");
  git_tag *tag;
  const char *tmessage;

  git_oid_mkstr(&oid, "bc422d45275aca289c51d79830b45cecebff7c3a");

  error = git_tag_lookup(&tag, repo, &oid);

  git_tag_target((git_object **)&commit, tag);
  git_tag_name(tag);    // "test"
  git_tag_type(tag);    // GIT_OBJ_TAG
  tmessage = git_tag_message(tag); // "tag message\n"
  printf("Tag Message: %s\n", tmessage);

  /* ---------------------------- */

  printf("\n*Tree Parsing*\n");

  git_tree *tree;
  git_tree_entry *entry;
  git_object *objt;

  git_oid_mkstr(&oid, "2a741c18ac5ff082a7caaec6e74db3075a1906b5");

  git_tree_lookup(&tree, repo, &oid);

  int cnt = git_tree_entrycount(tree); // 3
  printf("tree entries: %d\n", cnt);

  entry = git_tree_entry_byname(tree, "hello.c");
  git_tree_entry_name(entry); // "README"

  entry = git_tree_entry_byindex(tree, 0);
  printf("Entry name: %s\n", git_tree_entry_name(entry)); // "README"

  git_tree_entry_2object(&objt, repo, entry); // blob

  /* ---------------------------- */

  printf("\n*Blob Parsing*\n");

  git_blob *blob;

  git_oid_mkstr(&oid, "af7574ea73f7b166f869ef1a39be126d9a186ae0");

  git_blob_lookup(&blob, repo, &oid);
  printf("Blob Size: %d\n", git_blob_rawsize(blob)); // 8
  git_blob_rawcontent(blob); // "content"

  /* ---------------------------- */

  printf("\n*Revwalk Parsing*\n");
  git_revwalk *walk;
  git_commit *wcommit;

  git_oid_mkstr(&oid, "f0877d0b841d75172ec404fc9370173dfffc20d1");

  git_revwalk_new(&walk, repo);
  git_revwalk_sorting(walk, GIT_SORT_TOPOLOGICAL | GIT_SORT_REVERSE);
  git_revwalk_push(walk, &oid);

  const git_signature *cauth;
  const char *cmsg;

  while ((git_revwalk_next(&oid, walk)) == GIT_SUCCESS) {
    error = git_commit_lookup(&wcommit, repo, &oid);
    cmsg  = git_commit_message_short(wcommit);
    cauth = git_commit_author(wcommit);
    printf("%s (%s)\n", cmsg, cauth->email);
  }

  git_revwalk_free(walk);

  /* ---------------------------- */

  git_repository_free(repo);
}

