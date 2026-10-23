## Inspiration

We're a team of students. Before every exam we see the same problem: the material for one course is spread across lecture recordings on someone's phone, slide PDFs from the teacher, and photos of the whiteboard. Turning that into revision notes means replaying hours of audio and cross-checking slides by hand. Generic AI summarizers help a little, but they only look at one file at a time, need cloud uploads, and miss the one thing students care about most: *what did the lecturer say will be in the exam?*

A UGREEN AI NAS already stores all of this in one place and can transcribe, read and write locally. That made it the right home for a study assistant.

## What it does

**Lecture Revision Kit** is a Uliya Skill. A student says something like *"Make revision notes for my Operating Systems course"*, and Uliya:

1. Finds the course's recordings, slides and whiteboard photos and groups them by lecture.
2. Shows the list so the student can confirm it.
3. Transcribes the recordings, reads text from the board photos, and outlines the slides.
4. Writes **one note per lecture** with key topics, definitions, formulas, worked examples, *Lecturer emphasis* and *Conflicts and gaps*.
5. Writes **one exam revision sheet** with ranked topics, 10 practice questions and a quick-check list.
6. Saves everything to a `Revision` folder, but only after the student confirms.
7. On later runs, finds the earlier notes and processes only the new lectures, so it keeps up with the semester.

Every point carries its source, such as `[Rec 14:32]`, `[Slides p.7]` or `[Board IMG_2041]`, so nothing has to be taken on trust.

## How we built it

The Skill is a Markdown orchestration workflow built with the official **UGOS Skill Creation Tool** (`uliya-skill-kit`). We used Claude Code as our AI assistant, as the starter kit recommends. Uliya delegates each step to a registered NAS expert:

| Step | Expert | Capability |
|---|---|---|
| Find course files | `file-agent` | `ug-filesystem` |
| Transcribe recordings | `knowledge-agent` | `ug-knowledge` |
| Read whiteboard photos | `photo-agent` | `ug-photo` |
| Outline slides, write notes | `document-agent` | `ug-document` |
| Save after confirmation | `knowledge-agent`, `file-agent` | `ug-knowledge`, `ug-filesystem` |

Each step passes only the real result of the previous one. All five delegation states (`completed`, `failed`, `cancelled`, `waiting_user`, `waiting_approval`) and empty or partial results are handled explicitly. The package passes the official static checker with zero errors and zero warnings.

## Challenges

- **Wrong first idea.** We started by writing a duplicate-file cleaner in C. Then we read the rules: Skills are Markdown only, with no scripts or binaries, and duplicate cleanup is UGREEN's own example. We dropped it and picked a problem we understand from student life.
- **Designing for failure, not just success.** A recording may fail to transcribe, a board photo may be blurry, or a lecture may have no slides. The Skill reports each lecture as complete or partial and never fills gaps with invented content.
- **Safety by design.** Original files are read-only. New files are saved only after confirmation, in one chosen folder, and never overwrite anything.

## What we learned

- How AI agent orchestration works: splitting a job by expert responsibility and passing verified results between steps.
- That handling empty results, partial results and approvals is most of the real work in an AI workflow.
- How much a local AI NAS changes privacy: lecture recordings and notes never leave the device.

## What's next

- Test on a real iDX6011 Pro in Round 2: long 60–90 minute recordings, handwritten boards, and long transcripts.
- Support video lectures directly once a transcription path for video is available.
- Spaced-repetition review: resurface the topics the lecturer stressed most as the exam date gets closer.
