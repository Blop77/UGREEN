---
name: lecture-revision-kit
description: Lecture notes and an exam revision sheet for one course, built from the lecture recordings, slide or handout documents, and whiteboard photos already stored on the NAS, with every point traced back to its source. Use when a student says things like "make revision notes for my Operating Systems course", "turn this week's lectures into notes", or "help me revise for my exam from my lecture recordings and slides".
---

# Goal

Turn one course's study material that already sits on the NAS into study-ready text for a student:

- One **lecture note** per lecture that merges what was said (recording transcript), what was shown (slides or handouts), and what was written on the board (whiteboard photos).
- One **exam revision sheet** for the whole selection: key topics, definitions, formulas, points the lecturer stressed, and practice questions with answers that point back to their source.

Every point in the output names its source (Recording with time, Slides with page, or Board photo with file name). The Skill only reads the original study files. It never moves, renames, edits, or deletes them. The only write is saving the new note files, and only after the user confirms.

Supported scope: one course folder per run, up to 12 lectures per run. Audio recordings, slide or handout documents, and photos are supported sources. Video files are listed but not transcribed. Users with video lectures are told to export the audio track first.

For the README (user guide, examples, and how to remove the Skill), read [references/README.md](references/README.md). For dependencies, third-party, license, and AI tool disclosures, read [references/disclosures.md](references/disclosures.md).

# Required input

Collect these values before the first delegation. Use the exact values the user gives. Do not guess them.

1. `{course_folder}`: the NAS folder that holds this course. If the user gives only a course name, run Step 1a to look for matching folders and let the user pick one.
2. `{scope}`: the whole course folder, a list of lecture names or numbers, or a date range. Default: the whole course folder.
3. `{recording_language}`: the spoken language of the recordings, for example English, Hindi, or Hindi and English mixed. Default: English.
4. `{notes_language}`: the language the notes are written in. Default: English. It may differ from `{recording_language}`.
5. `{output_folder}`: where the new notes are saved. Default: a subfolder named `Revision` inside `{course_folder}`. Always confirm it with the user before Step 7.

Do not collect file IDs, CLI parameters, or other private parameters of the child experts. They select those themselves.

Values filled in during the run:

- `{course_name}`: the name the user used for the course, or else the last part of `{course_folder}`.
- `{confirmed_inventory}`: the lecture groups and file paths the user confirmed in Step 2.
- `{lecture_name}`, `{recording_path}`, `{photo_path}`, `{document_path}`: taken from `{confirmed_inventory}`.
- `{transcript}`, `{board_text}`, `{slide_outline}`: the real results of Steps 3, 4 and 5 for that lecture. For a long recording, `{transcript}` is the joined condensed parts from Step 3. Use the word none for a source the lecture does not have.
- `{transcript_part}`: one part of a long transcript, cut at a segment boundary in Step 3.
- `{existing_notes}`: the real text of earlier note files read in Step 1c, each headed by its lecture name. Use the word none on a first run.
- `{run_mode}`: `new lectures only` or `rebuild all`, chosen by the user in Step 1c. Default on a first run: `rebuild all`.
- `{lecture_note}`: the real notes for one lecture returned by the first delegation of Step 6.
- `{lecture_notes}`: all `{lecture_note}` results from Step 6, each headed by its lecture name.
- `{revision_sheet}`: the real revision sheet returned by the second delegation of Step 6.
- `{file_name}` and `{note_text}`: the confirmed file name from Step 7 and the matching text, either one `{lecture_note}` or the `{revision_sheet}`.

Before each delegation, replace every `{name}` in the instruction with the real value: either the user's answer above or the real result returned by the named upstream step. When a value contains a single quote, escape it with the standard shell sequence for a single quote inside single quotes. If a needed value is not available, stop the dependent step and tell the user which value is missing.

# Generation-time capability evidence

- `ug-filesystem` → `file-agent`
- `ug-knowledge` → `knowledge-agent`
- `ug-photo` → `photo-agent`
- `ug-document` → `document-agent`

# Workflow

Run the steps in this order. Each step uses only the real results returned by the steps before it.

## Step 1a: Find the course folder (only if the user gave a course name instead of a folder)

`delegate --target file-agent --instruction 'Search the NAS folders this user is allowed to access for directories whose name contains {course_name}. Return at most 10 matches, each with its full path and last-modified time. Only search; do not create, move, rename, or delete anything. If there are no matches, say so and return an empty list.'`

- If the result is an empty list (no results), tell the user no folder matched, ask for the folder path, and stop until they answer.
- If there are several matches, show them and ask the user to pick one. Use that path as `{course_folder}`.

## Step 1b: Build the lecture inventory

`delegate --target file-agent --instruction 'In the folder {course_folder} and its subfolders (max depth 3), list the files that match the scope {scope}. Include three groups: audio recordings, documents (PDF, PPT, PPTX, DOC, DOCX), and images (JPG, JPEG, PNG, HEIC). Also list video files separately, but do not process them. For each file return its full path, type, size, and last-modified time. Group files into lectures: files share a lecture when their names contain the same lecture number or date, or when they were last modified on the same day. Put files that match no group in a group named Unmatched. Return at most 12 lecture groups, sorted by date. Only read; do not create, move, rename, or delete anything. If no matching files are found, return an empty list and say so.'`

- If the inventory is an empty result, tell the user no lecture files were found in `{course_folder}` for `{scope}`, and stop. Do not search other folders and do not widen the scope on your own.
- If more than 12 lecture groups exist, say so and ask the user to narrow `{scope}`.

## Step 1c: Check for notes from earlier runs (repeat use)

`delegate --target file-agent --instruction 'Check whether the folder {output_folder} exists. If it exists, list the Markdown files in it whose names contain - notes, including numbered copies such as - notes 2, with full path and last-modified time, and return the text content of each one. Only read; do not create, move, rename, or delete anything. If the folder does not exist or has no such files, return an empty list and say so.'`

- If the result is an empty list (no results), this is a first run: set `{existing_notes}` to none and `{run_mode}` to `rebuild all`.
- If earlier notes exist, match each one to a lecture in the Step 1b inventory by its lecture name. If a lecture has several note files, use only the most recently modified one. Ask the user to choose `new lectures only` (default: keep the earlier notes and process only lectures that have no note yet) or `rebuild all`. This is a `waiting_user` point. Keep the real text of the earlier notes as `{existing_notes}`.
- If `{run_mode}` is `new lectures only` and every lecture in the inventory already has a note, tell the user there is nothing new to process. Offer to rebuild only the revision sheet from `{existing_notes}`; if the user agrees, go to the second delegation of Step 6. Otherwise finish as `completed` with nothing saved.

## Step 2: Confirm the inventory with the user

Show the real inventory from Step 1b as a table: lecture, recordings, documents, photos, video files that will not be processed, and whether the lecture already has a note from an earlier run. In `new lectures only` mode, lectures that already have a note are shown but not processed again. Ask the user to confirm or correct the grouping and to remove anything that is not study material. This is a `waiting_user` point. Continue only with the confirmed list `{confirmed_inventory}`.

## Step 3: Transcribe recordings

For each recording path in `{confirmed_inventory}`, one at a time:

`delegate --target knowledge-agent --instruction 'Transcribe the audio file {recording_path} in {recording_language}. Return the full transcript text with segment start times, plus any quality warnings or failed parts. Do not summarize, translate, or save anything. If the file cannot be transcribed, return failed with the reason.'`

Keep each real transcript as `{transcript}` for its lecture. If a recording fails, record it as missing for that lecture and continue with the other sources.

If a transcript is longer than about 6,000 words, condense it before Step 6 so the material stays within what one instruction can carry. Cut it at segment boundaries into parts of about 4,000 words, and for each part in order:

`delegate --target document-agent --instruction 'Condense this part of a lecture transcript for study notes. Keep every definition, formula, worked example, and number. Keep every sentence where the lecturer says something is important, will be in the exam, or should be remembered, word for word. Keep the segment start time in front of every kept point. Remove greetings, repetition, and side talk. Do not add facts that are not in the text. Return Markdown text in the result and do not save any file. Transcript part: {transcript_part}.'`

Join the real condensed parts in order and use them as `{transcript}`. If any part fails, use the uncondensed text of that part and tell the user that the lecture was processed in full length. In the final report, list which lectures were condensed.

## Step 4: Read whiteboard photos

For each image path in `{confirmed_inventory}`, one at a time:

`delegate --target photo-agent --instruction 'Run OCR on the photo {photo_path} and return the recognized text exactly as read, in reading order. Mark any parts that are unclear. Do not describe the scene, do not add text that is not in the photo, and do not change the photo or any album. If no text is recognized, return an empty result and say so.'`

Keep each real OCR text as `{board_text}` with its file name. A photo with no text (an empty result) is skipped and listed in the final report.

## Step 5: Outline slides and handouts

For each document path in `{confirmed_inventory}`:

`delegate --target document-agent --instruction 'Read the document {document_path} and return an outline of its content: section titles, key definitions, formulas, and worked examples, each with its page or slide number. Only use what is in the document. Do not write or save any file.'`

Keep each real outline as `{slide_outline}` with its file name.

## Step 6: Draft lecture notes and the revision sheet

For each lecture that has at least one real source from Steps 3 to 5:

`delegate --target document-agent --instruction 'Write study notes for the lecture {lecture_name} of the course {course_name} using only the material below. Use these sections in order: Summary (5 sentences max), Key topics, Definitions, Formulas, Worked examples, Lecturer emphasis (sentences where the lecturer says something is important, will be in the exam, or should be remembered, each with its recording time), and Conflicts and gaps (places where the recording, slides, and board disagree or where a source is missing). After every point, add its source in brackets: Recording with time, Slides with page, or Board photo with file name. Write the notes in {notes_language}. Quote lecturer emphasis sentences in their original words, followed by a translation in {notes_language} when the languages differ. Do not add facts that are not in the material. Return the notes as Markdown text in the result and do not save any file. Material: transcript {transcript}; slide outlines {slide_outline}; board text {board_text}.'`

Keep each real result as `{lecture_note}`. Then, once for the whole course, build the revision sheet from the new notes plus, in `new lectures only` mode, the earlier notes in `{existing_notes}`. In `rebuild all` mode, use the word none for `{existing_notes}` in this step so no lecture is counted twice:

`delegate --target document-agent --instruction 'Write one exam revision sheet for the course {course_name} using only the lecture notes below. Use these sections in order: Top topics ranked by how often they appear and how strongly the lecturer stressed them, Must-know definitions and formulas, Lecturer emphasis across lectures, 10 practice questions with short answers, and a one-page quick-check list. Every item must keep the source brackets from the notes and name its lecture. Write the sheet in {notes_language}. Do not add facts that are not in the notes. Return the sheet as Markdown text in the result and do not save any file. New lecture notes: {lecture_notes}. Earlier lecture notes: {existing_notes}.'`

Keep the real result as `{revision_sheet}`.

## Step 7: Preview and confirm saving

Show the user a short preview: the list of notes that will be saved, the first lines of the revision sheet, and the full `{output_folder}` path with the planned file names:

- `{lecture_name} - notes.md` for each lecture
- `{course_name} - revision sheet.md`

Earlier note files are never overwritten. If a planned name already exists, say that the new file will be saved with a number added to its name. Ask the user to confirm the folder and file names. This is a `waiting_user` point. If the user declines, do not save anything, show the full text in the conversation instead, and finish as `cancelled`.

## Step 8: Save the notes (only after confirmation)

First make sure the confirmed output folder exists:

`delegate --target file-agent --instruction 'Check whether the folder {output_folder} exists. If it does not exist, create only that folder, including missing parent folders inside {course_folder}. Do not create, move, rename, or delete anything else. Return the real folder path and whether it already existed.'`

If this step does not return `completed` with a real folder path, do not save any file and report the reason.

Then, for each confirmed file:

`delegate --target knowledge-agent --instruction 'Save the Markdown text below as a new file in the existing folder {output_folder} using the file name hint {file_name}. Do not overwrite an existing file: if the name is taken, add a number to the new file name. Do not change any other file. Return the real saved file path and whether the save was verified. Text: {note_text}.'`

The platform may ask the user to approve each save. Wait for that approval and do not save in any other way.

## Step 9: Report

Report only real results:

- The run mode, and the lectures left as they were because they already had a note.
- Each saved file with its real path from Step 8.
- For each lecture, which sources were used and which were missing or failed.
- Video files that were found but not processed.
- Any transcription quality warnings or unclear OCR text, and which long lectures were condensed.

# Completion and failure

Handle the five delegation states from every step like this:

- `completed`: use the real result in the next step. Report a business result only after the responsible expert returns `completed`. Never claim that a file was saved unless Step 8 returned `completed` with a real saved path.
- `failed`: report which step and which file failed, with the reason the expert gave. For Steps 3 to 5, continue with the remaining sources and mark that lecture as partial. If Step 1b or Step 6 fails, stop the steps that depend on it and say what was finished. If Step 1c fails, tell the user that earlier notes could not be read and ask whether to continue in `rebuild all` mode; this is a `waiting_user` point.
- `cancelled`: stop the workflow. Report what was already produced, and state that nothing more will be saved.
- `waiting_user`: show the expert's question to the user exactly, wait for the answer, then resume the same step with the answer.
- `waiting_approval`: tell the user that approval is needed and what will change (for example, one new note file in `{output_folder}`). Resume only after the platform reports the approval. If approval is denied, treat the step as `cancelled` for that file.

Empty and partial results:

- If the inventory is an empty result (no results), say so and stop. Do not switch to other folders or similar files.
- If a lecture has no usable source after Steps 3 to 5, leave it out of Step 6 and list it in the report as having no usable material.
- If only some lectures succeed, save and report those, and clearly list the lectures that are incomplete.
- Never invent lecture content, recording times, page numbers, or file paths. Every source reference must come from a real result.

Safety:

- The original recordings, documents, and photos are read only. They are never moved, renamed, edited, or deleted.
- New files are written only in the confirmed `{output_folder}`, only after the user confirms, and never over an existing file.
- No data is sent outside the NAS. All work is done by the NAS experts listed above.
- The Skill must not run cli.invoke directly. All NAS work goes through the `delegate` commands above.
