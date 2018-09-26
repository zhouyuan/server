class Opt_trace_ctx
{
public:
  ~Opt_trace_ctx();
private:
  Dynamic_array<Opt_trace_info *> traces;
  Opt_trace_info *current_trace;
  Json_writer *current_Json();
};