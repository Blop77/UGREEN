# Lecture Revision Kit: user guide

Version 0.9.0 (Round 1 pre-test package). See [Version history](#version-history).

## Who it is for

College and school students who keep their course material on a UGREEN NAS: lecture recordings from a phone or voice recorder, slides and handouts from the teacher, and photos of the whiteboard. Before an exam, these sit in three different formats, and turning them into notes by hand takes hours.

## What you get

- **One note per lecture** that combines what was said, what was on the slides, and what was written on the board.
- **One revision sheet** for the course: top topics, definitions, formulas, the points your lecturer stressed, 10 practice questions with answers, and a quick-check list.
- **Every point shows its source**, for example `[Recording 14:32]`, `[Slides p.7]` or `[Board photo IMG_2041.jpg]`, so you can check anything quickly.
- A **Conflicts and gaps** section that tells you when the slides and the lecture disagree, or when a source is missing.

## Requirements and setup

- A UGREEN AI NAS running UGOS with Uliya and its built-in file, knowledge, photo and document experts.
- No accounts, API keys, add-ons or settings. There is nothing to configure.
- Your course material in one folder per course on the NAS.

## Install

Install `lecture-revision-kit` the same way as any other Uliya Skill: add it from UGREEN Skills Hub, or upload the Skill package where UGOS lets you add Skills. Then ask Uliya one of the example requests below to check it responds.

## Permissions

- **Read** access to the course folder you choose. Uliya only reads the files there.
- **Write** access only to the output folder (default: `Revision` inside the course folder). Every save is shown in a preview first, and UGOS asks you to approve it.
- Nothing is sent outside the NAS.

## How to use it

Say one of these to Uliya:

- "Make revision notes for my Operating Systems course."
- "Turn lectures 5 to 8 of Data Structures into notes."
- "Help me revise for my DBMS exam from my recordings and slides."

Uliya then:

1. Asks for the course folder (or finds it from the course name), which lectures to include, the language of the recordings, and the language you want the notes in.
2. Shows a table of the recordings, documents and photos it found, grouped by lecture, and asks you to confirm.
3. Transcribes the recordings, reads the board photos, and outlines the slides.
4. Writes the lecture notes and the revision sheet.
5. Shows a preview and asks where to save them. The default is a `Revision` folder inside the course folder.
6. Saves the notes after you confirm, and reports what was saved and what was missing.

## Keep your notes up to date during the semester

Run it again whenever you add new lectures, for example once a week. Uliya finds the notes it saved earlier and asks you to choose:

- **New lectures only** (default): earlier notes are kept, only lectures without a note are processed, and a fresh revision sheet is built from all notes, old and new.
- **Rebuild all**: every lecture is processed again. Earlier files are kept, and the new ones are saved with a number added to the name.

If there is nothing new, Uliya tells you so and offers to rebuild only the revision sheet.

## Tips for best results

- Keep one folder per course, and put the lecture number or date in file names, for example `Lecture 05 - Scheduling.m4a` and `Lecture 05 - slides.pdf`.
- Board photos work best when taken straight on, in good light.
- Video lectures: export the audio track first. Video files are listed but not transcribed.
- Up to 12 lectures per run. For a full semester, run it once per unit.
- Lectures in Hindi, or a mix of Hindi and English, can still give English notes: say the recording language and ask for notes in English. The lecturer's key sentences are quoted as spoken, with a translation.
- Long lectures (over about 45 minutes) are condensed in parts first, so they take a little longer. Every definition, formula, example and exam hint is kept.

## What it never does

- It never moves, renames, edits or deletes your original files.
- It never overwrites an existing file.
- It never sends your files or notes outside your NAS.
- It never adds facts that are not in your own material. Practice questions are built only from your notes.

## Testing it

Static check: this package passes the official UGOS Skill Creation Tool checker (uliya-skill-kit 0.7.3) with 0 errors and 0 warnings. It has not yet run on a real NAS; that happens in Round 2.

Planned tests on a NAS, using a sample course with known answers:

1. **One complete lecture** (recording, slides, board photo): every point in the note has a valid source, and a planted 4 KB versus 8 KB page-size conflict appears under Conflicts and gaps.
2. **Whole course, then a repeat run:** the revision sheet ranks the topics the lecturer stressed. After a new lecture is added, New lectures only processes just that lecture, and no earlier file is overwritten.
3. **Edge case:** a lecture with only a video and an unreadable photo is reported as having no usable material, nothing is invented, and nothing is saved without confirmation.

## Removing it

Remove the Skill from Uliya's Skill list. The notes it saved are ordinary Markdown files in your output folder. Keep or delete them as you like.

## Version history

- **0.9.0** (Round 1): full workflow with source tracing, lecturer emphasis, conflicts and gaps, revision sheet, repeat-use mode, notes language, and condensing of long transcripts.
