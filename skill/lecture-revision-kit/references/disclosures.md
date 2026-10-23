# Disclosures

## Package contents

- `SKILL.md`: the Skill entry point. Its frontmatter (`name`, `description`) is the manifest, and its body is the full source of the workflow.
- `references/README.md`: user guide.
- `references/disclosures.md`: this file.

No configuration template is needed: the Skill has no settings, credentials, or keys.

## Dependencies

The Skill depends only on capabilities built into UGREEN AI NAS, delegated through Uliya:

| Capability | Expert | Used for |
|---|---|---|
| `ug-filesystem` | `file-agent` | Finding course files and earlier notes, creating the output folder |
| `ug-knowledge` | `knowledge-agent` | Transcribing recordings, saving notes |
| `ug-photo` | `photo-agent` | Reading text from whiteboard photos |
| `ug-document` | `document-agent` | Outlining slides, condensing long transcripts, writing notes and the revision sheet |

There are no external services, network calls, packages, or downloads.

## Third-party components and open source libraries

None. There are no third-party components or open source libraries, so there are no third-party sources or licenses to list. This Skill is made only of Markdown instructions. It contains no code, libraries, scripts, binaries, models, or downloads. All work is done by the UGREEN AI NAS experts and capabilities listed in SKILL.md.

## Principal functionality developed for this Hackathon

All of the following was newly developed during the UGREEN Hackathon 2026 on AI NAS:

- The end-to-end workflow that combines three kinds of lecture material (recordings, slides or handouts, and whiteboard photos) into one note per lecture.
- Source tracing: every point in the notes and revision sheet names its source (recording time, slide page, or photo file name).
- Lecturer-emphasis detection and the Conflicts and gaps section that compares the three sources.
- The exam revision sheet format with ranked topics, practice questions and a quick-check list.
- Notes in a different language from the lecture, with the lecturer's key sentences quoted in the original words.
- Condensing long lecture transcripts in parts while keeping definitions, formulas, examples and exam hints word for word.
- Repeat use during a semester: earlier notes are detected, only new lectures are processed, and the revision sheet is rebuilt from old and new notes.
- The confirmation, read-only, no-overwrite and partial-result rules.

## AI tools used and their principal uses

- **Claude Code (Anthropic):** helped draft and revise SKILL.md, this reference, and the user guide, and ran the static check.
- **UGOS Skill Creation Tool (uliya-skill-kit 0.7.3, provided by UGREEN for the Hackathon):** capability catalog for feasibility checks and the controlled static checker.
