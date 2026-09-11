#!/usr/bin/env python3
"""
export_chat.py
Exports the full conversation transcript from Antigravity IDE into a Markdown file
at the root of the workspace.
"""

import os
import json
import re

TRANSCRIPT_PATH = r"C:\Users\dhenriquez\.gemini\antigravity-ide\brain\9a3276ba-cb5c-482d-9346-5dd0e5f2de57\.system_generated\logs\transcript_full.jsonl"
OUTPUT_MD = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "CHAT.md")

def clean_user_text(raw):
    # Extract between <USER_REQUEST> and </USER_REQUEST>
    m = re.search(r"<USER_REQUEST>(.*?)</USER_REQUEST>", raw, re.DOTALL)
    if m:
        text = m.group(1).strip()
    else:
        text = re.sub(r"<ADDITIONAL_METADATA>.*", "", raw, flags=re.DOTALL).strip()
    return text

def main():
    if not os.path.exists(TRANSCRIPT_PATH):
        print(f"Error: Transcript file not found at {TRANSCRIPT_PATH}")
        return

    entries = []
    with open(TRANSCRIPT_PATH, "r", encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            try:
                item = json.loads(line)
            except Exception as e:
                continue

            t = item.get("type")
            content = item.get("content", "")
            timestamp = item.get("timestamp", "")

            if t == "USER_INPUT":
                text = clean_user_text(content)
                if text:
                    entries.append({
                        "role": "Usuario",
                        "text": text,
                        "time": timestamp
                    })
            elif t == "PLANNER_RESPONSE" and content:
                text = content.strip()
                if text:
                    # Avoid adding duplicate consecutive assistant messages if identical
                    if entries and entries[-1]["role"] == "Asistente" and entries[-1]["text"] == text:
                        continue
                    entries.append({
                        "role": "Asistente",
                        "text": text,
                        "time": timestamp
                    })

    # Build Markdown document
    md_lines = []
    md_lines.append("# 📜 Historial Completo de la Conversación")
    md_lines.append("")
    md_lines.append("> **Proyecto**: ESP32 Game Boy Pokémon Trading con Portal Cautivo")
    md_lines.append("> **Fecha de exportación**: 2026-09-10")
    md_lines.append("> **Participantes**: Usuario & Antigravity (Asistente de IA)")
    md_lines.append("")
    md_lines.append("---")
    md_lines.append("")

    turn_idx = 1
    for entry in entries:
        role = entry["role"]
        text = entry["text"]

        if role == "Usuario":
            md_lines.append(f"## 👤 Turno {turn_idx}: Usuario")
            md_lines.append("")
            md_lines.append(f"```text\n{text}\n```")
            md_lines.append("")
            turn_idx += 1
        else:
            md_lines.append("### 🤖 Asistente (Antigravity)")
            md_lines.append("")
            md_lines.append(text)
            md_lines.append("")
            md_lines.append("---")
            md_lines.append("")

    with open(OUTPUT_MD, "w", encoding="utf-8") as f:
        f.write("\n".join(md_lines))

    print(f"Exported {len(entries)} turns to {OUTPUT_MD}")

if __name__ == "__main__":
    main()
