/*

на основе 
Mongoose networking library 
https://mongoose.ws/documentation/

исходники Mongoose
https://github.com/cesanta/mongoose.git

*/


#include "mongoose.h"

const char *s_listening_url = "http://0.0.0.0:8000";

#define DF_AR_LAMP_COUNT 3
static unsigned char arLamp[DF_AR_LAMP_COUNT]={0,0,0};

// Mongoose event handler function, gets called by the mg_mgr_poll()

// HTTP request handler function. It implements the following endpoints:
//   /api/log/static - returns contents of log.txt file
//   /api/log/live - hangs forever, and returns live log messages
//   all other URI - serves web_root/ directory
static void
fn (struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
  if (ev == MG_EV_HTTP_MSG)
    {
      struct mg_http_message *hm = (struct mg_http_message *)ev_data;
      if (mg_http_match_uri (hm, "/api/log/static"))
        {
          struct mg_http_serve_opts opts
              = { .root_dir = "/web_root", .fs = &mg_fs_packed };
          mg_http_serve_file (c, hm, "/web_root/log.txt", &opts);
        }
      else if (mg_http_match_uri (hm, "/api/log/live"))
        {
          c->data[0] = 'L'; // Mark that connection as live log listener
          mg_printf (c,
                     "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
        }

      else if (mg_http_match_uri (hm, "/api/sost"))
        {
          int i_lamp=255;
          if(hm->query.len>0)i_lamp=hm->query.ptr[0]-48;
          if(i_lamp<DF_AR_LAMP_COUNT){MG_INFO(("lamp %d sost %d",i_lamp,arLamp[i_lamp]));}
          mg_http_reply(c, 200, "", "%u", (i_lamp<DF_AR_LAMP_COUNT?arLamp[i_lamp]:255) );
        }
      else if (mg_http_match_uri (hm, "/api/on"))
        {
          int i_lamp=255;
          if(hm->query.len>0)i_lamp=hm->query.ptr[0]-48;
          if(i_lamp<DF_AR_LAMP_COUNT){arLamp[i_lamp]=1;MG_INFO(("lamp %d on",i_lamp));}
          mg_http_reply(c, 200, "", "{%m:%u}\n",MG_ESC("on"), i_lamp );
        }
      else if (mg_http_match_uri (hm, "/api/off"))
        {
          int i_lamp=255;
          if(hm->query.len>0)i_lamp=hm->query.ptr[0]-48;
          if(i_lamp<DF_AR_LAMP_COUNT){arLamp[i_lamp]=0;MG_INFO(("lamp %d off",i_lamp));}
          mg_http_reply(c, 200, "", "{%m:%u}\n",MG_ESC("off"), i_lamp);
        }

      else
        {
          struct mg_http_serve_opts opts
              = { .root_dir = "/web_root", .fs = &mg_fs_packed };
          mg_http_serve_dir (c, ev_data, &opts);
        }
    }
  (void)fn_data;
}

static void
broadcast_message (struct mg_mgr *mgr, const char *message)
{
  struct mg_connection *c;
  for (c = mgr->conns; c != NULL; c = c->next)
    {
      if (c->data[0] == 'L')
        mg_http_printf_chunk (c, "%s", message);
    }
}

// Timer function - called periodically.
// Prepare log message. Save it to a file, and broadcast.
static void
timer_fn (void *arg)
{
  char buf[64];
  snprintf (buf, sizeof (buf), "mg_millis is: %lu\n", (unsigned long)mg_millis());
  broadcast_message (arg, buf);
}


int main(void) {
  struct mg_mgr mgr;
  mg_mgr_init(&mgr);
  mg_log_set(MG_LL_INFO);
  mg_timer_add (&mgr, 1000, MG_TIMER_REPEAT, timer_fn, &mgr);

  mg_http_listen(&mgr, s_listening_url, fn, NULL);
  while (true) mg_mgr_poll(&mgr, 500);
  mg_mgr_free(&mgr);
  return 0;
}
