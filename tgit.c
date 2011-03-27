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
  char out[40];
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

  git_repository_free(repo);
}

