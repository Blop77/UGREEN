#ifndef NASWEEP_AI_H
#define NASWEEP_AI_H

#include "dupes.h"

/*
 * Ask a local LLM (llama.cpp server or Ollama, OpenAI-compatible API)
 * to explain the duplicate report in plain language.
 * Returns 0 on success.
 */
int ai_explain(const dupe_report *report, const file_list *files, const char *endpoint);

#endif
