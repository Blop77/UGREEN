# Round 2 test-day plan

Finalists get **one assigned day** of remote access to a UGREEN AI NAS iDX6011 Pro. This plan is meant to get every test and all the demo footage done in that one day.

## Before test day (at home)

- [ ] Record `Lecture 01 - recording.m4a` and `Lecture 02 - recording.m4a` from [testkit/recording-scripts.md](../testkit/recording-scripts.md) and put them in `testkit/Operating Systems/`.
- [ ] Rebuild the Skill ZIP: `make package`.
- [ ] Install screen recording software (OBS Studio is free) and do a short practice recording.
- [ ] Print or open this checklist on a second screen.

Test course contents, with what each file is for:

| File | Purpose |
|---|---|
| `Lecture 01 - slides.pdf`, `- board.jpg`, `- recording.m4a` | Complete lecture: all three sources |
| `Lecture 02 - slides.pdf`, `- board.jpg`, `- recording.m4a` | Slides say 4 KB pages, recording says 8 KB (conflict); exam hint in speech and on board |
| `Lecture 03 - recording.mp4`, `- board.jpg` | Edge case: video only plus an unreadable (blurred) board photo |
| `Course outline.pdf` | No lecture number, so it should land in the Unmatched group |
| `add-later/Lecture 04 - slides.pdf` | Copied in later for the repeat-use test |

## Test day schedule

| Time | Task |
|---|---|
| First 30 min | Log in, install the Skill, upload `Operating Systems` (without `add-later`) to the NAS |
| 1 hour | Tests 1–2 (core) |
| 1 hour | Tests 3–4 (repeat use and edge case) |
| 1 hour | Fix anything that failed, re-check with `make check`, reinstall, re-run the failed test |
| 1 hour | Record the demo video (see [demo-video-script.md](demo-video-script.md)) |
| Spare | Re-takes and saving all evidence |

Screen-record **every** test run, not just the demo. A failed run is still useful evidence of error handling.

## Test 1 (core): one complete lecture

Say: *"Make revision notes for Lecture 02 of my Operating Systems course."*

| Check | Pass? |
|---|---|
| Uliya finds the course folder, or asks for it | |
| The inventory table shows Lecture 02 with slides, board photo and recording | |
| Uliya waits for confirmation before processing | |
| The note has all 7 sections in order | |
| Every point has a source: `[Recording mm:ss]`, `[Slides p.N]` or `[Board photo ...]` | |
| **Conflicts and gaps** mentions 4 KB (slides) versus 8 KB (recording) | |
| **Lecturer emphasis** includes "paging versus segmentation will definitely be in the exam" with a time | |
| Formula and the 120 ns example match the slides | |
| Preview is shown and nothing is saved before confirming | |
| After confirming, `Revision/Lecture 02 - notes.md` exists and the original files are unchanged | |

## Test 2 (core): course revision sheet

Say: *"Make revision notes for my Operating Systems course."* Choose **Rebuild all** if asked.

| Check | Pass? |
|---|---|
| Inventory groups Lectures 01, 02, 03, and puts `Course outline.pdf` in Unmatched | |
| `Lecture 03 - recording.mp4` is listed as a video that will not be processed | |
| Revision sheet has ranked topics, 10 practice questions and a quick-check list | |
| Both exam hints (context switch, paging vs segmentation) appear under Lecturer emphasis | |
| The existing `Lecture 02 - notes.md` from Test 1 is **not** overwritten; the new one gets a number | |

## Test 3 (repeat use): new lectures only

Copy `add-later/Lecture 04 - slides.pdf` into the course folder, then say: *"Update my Operating Systems revision notes."*

| Check | Pass? |
|---|---|
| Uliya detects the earlier notes and offers **New lectures only** or **Rebuild all** | |
| With New lectures only, only Lectures 03 and 04 are processed (03 has no note because it had no usable material) | |
| Lecture 04 note says its recording and board photo are missing (slides only) | |
| A new revision sheet covers Lectures 01, 02 and 04, and Lecture 03 is again reported as having no usable material | |
| Remove the Lecture 03 files and run again: Uliya says there is nothing new and offers to rebuild only the sheet | |

## Test 4 (edge): no usable material

Say: *"Make revision notes for Lecture 03 of my Operating Systems course."*

| Check | Pass? |
|---|---|
| The video is listed but not transcribed | |
| The blurred board photo returns no readable text and is reported | |
| Uliya says Lecture 03 has no usable material and does **not** invent a note | |
| Nothing is saved | |

Extra edge checks if time allows:

- Notes language: record Lecture 01 again in Hindi (or Hindi and English mixed) and ask for English notes. The exam hint should be quoted as spoken, with an English translation.

- Ask for a course folder that does not exist: Uliya should say no folder matched and stop.
- Decline the save at the preview: nothing should be saved, and the text is shown in the chat instead.

## Record results

After each test, note pass or fail and anything surprising in the Pass? column, and keep the screen recordings. Use the results to fill in the final submission and to adjust the Skill's wording.
