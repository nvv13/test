#ifndef LFILE_HTTP_H
#define LFILE_HTTP_H

#ifdef __cplusplus
extern "C"
{
#endif

  FRESULT download_file_http (const char *Uri, FIL *pfnew);

#ifdef __cplusplus
}
#endif

#endif /* LFILE_HTTP_H */
