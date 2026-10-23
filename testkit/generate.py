"""Build the Round 2 test course: slide PDFs, whiteboard photos and a video-only lecture.

Run from the repo root:  python3 testkit/generate.py
Needs Chromium (for PDFs), Pillow, and ffmpeg. Recordings are made by hand from
testkit/recording-scripts.md because no speech is synthesized here.
"""
import os
import subprocess
import tempfile

from PIL import Image, ImageDraw, ImageFilter, ImageFont

ROOT = os.path.dirname(os.path.abspath(__file__))
COURSE = os.path.join(ROOT, "Operating Systems")
LATER = os.path.join(ROOT, "add-later")
CHROME = os.environ.get("CHROME", "/opt/pw-browsers/chromium-1194/chrome-linux/chrome")
FONT = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"
FONT_BOLD = "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf"

SLIDES = {
    "Lecture 01 - slides.pdf": ("Lecture 01: Processes", [
        ("What is a process?", ["A process is a program in execution.",
                                "It has its own address space, registers and program counter."]),
        ("Process states", ["new, ready, running, waiting, terminated",
                            "The scheduler moves processes from ready to running."]),
        ("Process Control Block (PCB)", ["Stores state, program counter, registers, open files.",
                                         "A context switch saves one PCB and loads another."]),
    ]),
    "Lecture 02 - slides.pdf": ("Lecture 02: Memory Management", [
        ("Paging", ["Physical memory is split into fixed-size frames.",
                    "Logical memory is split into pages of the same size.",
                    "Page size in this course: 4 KB."]),
        ("Translation Lookaside Buffer (TLB)", ["A small cache of recent page-table entries.",
                                                "TLB hit ratio h = hits / total lookups."]),
        ("Effective Access Time", ["EAT = h(t_TLB + t_mem) + (1 - h)(t_TLB + 2 t_mem)",
                                   "Example: h = 0.9, t_TLB = 10 ns, t_mem = 100 ns gives EAT = 120 ns."]),
    ]),
    "Course outline.pdf": ("Operating Systems: Course Outline", [
        ("Units", ["1. Processes and threads", "2. Memory management",
                   "3. Scheduling", "4. File systems"]),
        ("Assessment", ["Mid-term exam 30%", "Lab work 20%", "Final exam 50%"]),
    ]),
}

LATER_SLIDES = {
    "Lecture 04 - slides.pdf": ("Lecture 04: CPU Scheduling", [
        ("Goals", ["Maximize CPU use and throughput.", "Minimize waiting and response time."]),
        ("Algorithms", ["FCFS: first come, first served.", "SJF: shortest job first.",
                        "Round Robin: fixed time quantum, then the next process runs."]),
    ]),
}

BOARDS = {
    "Lecture 01 - board.jpg": ["new -> ready -> running -> terminated",
                               "running -> waiting -> ready",
                               "Context switch = save PCB + load PCB"],
    "Lecture 02 - board.jpg": ["EAT = h(t_TLB + t_mem)",
                               "    + (1-h)(t_TLB + 2 t_mem)",
                               "IMPORTANT: paging vs segmentation!"],
    "Lecture 03 - board.jpg": ["Deadlock: 4 conditions",
                               "mutual exclusion, hold and wait,",
                               "no preemption, circular wait"],
}


def slides_pdf(path, title, pages):
    css = ("@page{size:254mm 143mm;margin:0}body{margin:0;font-family:'DejaVu Sans',sans-serif}"
           ".p{width:254mm;height:143mm;padding:14mm 18mm;box-sizing:border-box;page-break-after:always}"
           ".p:last-child{page-break-after:auto}h1{font-size:38pt;color:#1d3c78;margin:0 0 6mm}"
           "h2{font-size:30pt;color:#1d3c78;margin:0 0 6mm}li{font-size:22pt;margin:4mm 0}"
           ".n{position:absolute;right:10mm;bottom:6mm;font-size:10pt;color:#888}.p{position:relative}")
    body = f"<div class='p'><h1>{title}</h1><p style='font-size:16pt'>Operating Systems</p><div class='n'>1</div></div>"
    for i, (heading, bullets) in enumerate(pages, start=2):
        items = "".join(f"<li>{b}</li>" for b in bullets)
        body += f"<div class='p'><h2>{heading}</h2><ul>{items}</ul><div class='n'>{i}</div></div>"
    with tempfile.NamedTemporaryFile("w", suffix=".html", delete=False) as f:
        f.write(f"<!doctype html><meta charset='utf-8'><style>{css}</style>{body}")
    subprocess.run([CHROME, "--headless", "--no-sandbox", "--disable-gpu", "--no-pdf-header-footer",
                    f"--print-to-pdf={path}", "file://" + f.name],
                   check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    os.unlink(f.name)


def board_photo(path, lines, blur=0):
    img = Image.new("RGB", (1600, 1000), (236, 238, 232))
    d = ImageDraw.Draw(img)
    d.rectangle([40, 40, 1560, 960], outline=(150, 150, 150), width=12)
    font = ImageFont.truetype(FONT_BOLD, 64)
    for i, line in enumerate(lines):
        colour = (200, 40, 40) if "IMPORTANT" in line else (30, 60, 140)
        d.text((110, 160 + i * 180), line, font=font, fill=colour)
    img = img.rotate(-2.5, expand=False, fillcolor=(120, 120, 120))
    if blur:
        img = img.filter(ImageFilter.GaussianBlur(blur))
    img.save(path, quality=85)


def video_only(path):
    subprocess.run(["ffmpeg", "-y", "-loglevel", "error", "-f", "lavfi",
                    "-i", "color=c=0x1d3c78:s=640x360:d=6", "-vf",
                    f"drawtext=fontfile={FONT}:text='Lecture 03 (video only)':fontcolor=white:"
                    "fontsize=34:x=(w-text_w)/2:y=(h-text_h)/2",
                    "-c:v", "libx264", "-pix_fmt", "yuv420p", path], check=True)


def main():
    os.makedirs(COURSE, exist_ok=True)
    os.makedirs(LATER, exist_ok=True)
    for name, (title, pages) in SLIDES.items():
        slides_pdf(os.path.join(COURSE, name), title, pages)
    for name, (title, pages) in LATER_SLIDES.items():
        slides_pdf(os.path.join(LATER, name), title, pages)
    for name, lines in BOARDS.items():
        # Lecture 03's photo is deliberately unreadable to test the empty-OCR path.
        board_photo(os.path.join(COURSE, name), lines, blur=30 if name.startswith("Lecture 03") else 0)
    video_only(os.path.join(COURSE, "Lecture 03 - recording.mp4"))
    print("Test course written to", COURSE)


if __name__ == "__main__":
    main()
