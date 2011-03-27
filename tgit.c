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
  }

  /* ---------------------------- */

  git_repository_free(repo);
}

