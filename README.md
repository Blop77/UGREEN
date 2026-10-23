# Lecture Revision Kit

![Lecture Revision Kit](assets/thumbnail.png)

A Uliya Skill for UGREEN AI NAS that turns one course's lecture recordings,
slides and handouts, and whiteboard photos into one note per lecture plus an
exam revision sheet. Every point is traced back to its source (recording time,
slide page, or photo file name).

Entry for the **UGREEN Hackathon 2026 on AI NAS**, track *Automate Your Workflow*.

**Interactive demo:** https://claude.ai/artifact/HQG99SPCGsTxPYofMaPbP5 (a scripted walkthrough on the sample
course; the results are illustrative, not real NAS output). Source: `demo/index.html`.

## How it works

| Step | Expert | Capability |
|---|---|---|
| Find the course's recordings, documents and photos | `file-agent` | `ug-filesystem` |
| Transcribe lecture recordings | `knowledge-agent` | `ug-knowledge` |
| Read text from whiteboard photos | `photo-agent` | `ug-photo` |
| Outline slides, draft notes and the revision sheet | `document-agent` | `ug-document` |
| Save notes after the user confirms | `knowledge-agent`, `file-agent` | `ug-knowledge`, `ug-filesystem` |

Original files are never moved, edited or deleted. New notes are saved only
after confirmation and never overwrite existing files. On later runs it finds
the earlier notes and processes only new lectures.

## Repository layout

```
skill/lecture-revision-kit/   the Skill (this is what gets submitted)
  SKILL.md                    entry point: goal, inputs, workflow, failure handling
  references/user-guide.md    user guide
  references/disclosures.md   third-party, license and AI tool disclosures
assets/                       Devpost thumbnail and gallery images (1200x800)
docs/                         Round 1 proposal, Devpost story, Round 2 test plan and demo script
testkit/                      sample course for Round 2 testing (not part of the Skill)
tools/lint_skill.py           extra check: every {value} in SKILL.md is defined and used
demo/index.html               interactive walkthrough page (single file, no build step)
Makefile                      check and package helpers
```

## Check and package

Requires Node.js and the official UGOS Skill Creation Tool (`uliya-skill-kit`)
installed in your AI assistant's skills folder.

```
make check     # official static checker + our value lint
make package   # build dist/lecture-revision-kit.zip for Round 1
```
