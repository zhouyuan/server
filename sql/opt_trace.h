#ifndef OPT_TRACE_INCLUDED
#define OPT_TRACE_INCLUDED
/* This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA */

#include "opt_trace_context.h"  // Opt_trace_context
#include "sql_lex.h"
#include "my_json_writer.h"
class Item;
class THD;
struct TABLE_LIST;

class Opt_trace_stmt;

/*
   User-visible information about a trace.
*/

struct Opt_trace_info
{
  /**
     String containing trace.
     If trace has been end()ed, this is 0-terminated, which is only to aid
     debugging or unit testing; this property is not relied upon in normal
     server usage.
     If trace has not been ended, this is not 0-terminated. That rare case can
     happen when a substatement reads OPTIMIZER_TRACE (at that stage, the top
     statement is still executing so its trace is not ended yet, but may still
     be read by the sub-statement).
  */
  const char *trace_ptr;
  size_t trace_length;
  //// String containing original query.
  const char *query_ptr;
  size_t query_length;
  const CHARSET_INFO *query_charset;  ///< charset of query string
  /**
    How many bytes this trace is missing (for traces which were truncated
    because of @@@@optimizer-trace-max-mem-size).
    The trace is not extended beyond trace-max-mem-size.
  */
  size_t missing_bytes;
  /*
    Whether user lacks privilege to see this trace.
    If this is set to TRUE, then we return an empty trace
  */
  bool missing_priv;
};

/**
  Instantiate this class to start tracing a THD's actions (generally at a
  statement's start), and to set the "original" query (not transformed, as
  sent by client) for the new trace. Destructor will end the trace.

  @param  thd          the THD
  @param  tbl          list of tables read/written by the statement.
  @param  sql_command  SQL command being prepared or executed
  @param  set_vars     what variables are set by this command (only used if
                       sql_command is SQLCOM_SET_OPTION)
  @param  query        query
  @param  length       query's length
  @param  charset      charset which was used to encode this query
*/


class Opt_trace_start {
 public:
  Opt_trace_start(THD *thd_arg, TABLE_LIST *tbl,
                  enum enum_sql_command sql_command,
                  List<set_var_base> *set_vars,
                  const char *query,
                  size_t query_length,
                  const CHARSET_INFO *query_charset);
  ~Opt_trace_start();

 private:
  Opt_trace_context *const ctx;
  /*
    True: the query will be traced
    False: otherwise
  */
  bool traceable;
};

/**
   Prints SELECT query to optimizer trace. It is not the original query (as in
   @c Opt_trace_context::set_query()) but a printout of the parse tree
   (Item-s).
   @param  thd         the THD
   @param  select_lex  query's parse tree
   @param  trace_object  Json_writer object to which the query will be added
*/
void opt_trace_print_expanded_query(THD *thd, SELECT_LEX *select_lex,
                                    Json_writer_object *trace_object);
/**
   Fills information_schema.OPTIMIZER_TRACE with rows (one per trace)
   @retval 0 ok
   @retval 1 error
*/
int fill_optimizer_trace_info(THD *thd, TABLE_LIST *tables, Item *);
#endif