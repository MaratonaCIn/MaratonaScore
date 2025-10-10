import pandas as pd
import re
import json
import sys

# Optional dependency: ftfy is great at fixing mojibake. Use if available, otherwise
# we'll attempt safe re-encodings as fallbacks.
try:
    from ftfy import fix_text as _ftfy_fix
except Exception:
    _ftfy_fix = None

def convert_scoreboard_excel_to_json(input_filename, output_filename="scoreboard.json"):
    """
    Reads a scoreboard XLSX, cleans known data issues, processes the data,
    and saves it as a JSON file.
    
    Requirements:
        - pandas: `pip install pandas`
        - openpyxl: `pip install openpyxl`
    """
    try:
        # --- 1. Load the XLSX File ---
        df = pd.read_excel(input_filename, engine='openpyxl')
        print(f"✅ File '{input_filename}' loaded successfully.")

        # --- 2. Clean the Data ---
        df['Penalty'] = df['Penalty'].astype(str).str.extract(r'^(\d+)').astype(int)
        print("✅ 'Penalty' column has been cleaned.")

        # --- 3. Clean Up Column Names ---
        problem_columns = [col for col in df.columns if ' / ' in str(col)]
        column_rename_map = {col: col.split('\n')[0] for col in problem_columns}
        df.rename(columns=column_rename_map, inplace=True)
        problem_keys = list(column_rename_map.values())

        # --- 4. Helper Functions to Parse Data ---
        def parse_team_info(raw_team_str):
            if not isinstance(raw_team_str, str): return raw_team_str, None
            # repair possible mojibake/encoding problems in the team string first
            repaired = _repair_text(raw_team_str)
            match = re.match(r"([^(]+)\((.*?)\)", repaired)
            if match:
                team, user = match.groups()
                return team.strip(), user.strip()
            return repaired.strip(), None

        def parse_problem_cell(cell_content):
            cell_str = str(cell_content)
            time_match = re.search(r"(\d{1,2}:\d{2}:\d{2})", cell_str)
            penalty_match = re.search(r"\(-(\d+)\)", cell_str)
            is_solved = bool(time_match)
            penalty_attempts = int(penalty_match.group(1)) if penalty_match else 0
            return {
                "solved": is_solved, "time": time_match.group(1) if is_solved else None,
                "penalty_attempts": penalty_attempts,
                "total_attempts": penalty_attempts + 1 if is_solved else penalty_attempts,
            }

        # --- helper to repair mojibake in strings ---
        def _repair_text(s: str) -> str:
            if not isinstance(s, str):
                return s
            # 1) Try ftfy if available
            if _ftfy_fix:
                try:
                    fixed = _ftfy_fix(s)
                    # if ftfy changed something, return it
                    if fixed and fixed != s:
                        return fixed
                except Exception:
                    pass
            # 2) Try common re-encoding fallbacks. Many mojibake cases happen when
            # UTF-8 bytes were decoded as latin1/cp1252. Re-encode as latin1/cp1252
            # and decode as utf-8.
            for enc in ('latin1', 'cp1252'):
                try:
                    candidate = s.encode(enc, errors='strict').decode('utf-8', errors='strict')
                    if candidate and candidate != s:
                        return candidate
                except Exception:
                    # try with replacement (more forgiving)
                    try:
                        candidate = s.encode(enc, errors='replace').decode('utf-8', errors='replace')
                        if candidate and candidate != s:
                            return candidate
                    except Exception:
                        pass
            # 3) If we still have '?' or replacement character, try more heuristics
            if '?' in s or '\ufffd' in s:
                # Try decoding the UTF-8 bytes as cp1252 (reverse direction)
                try:
                    candidate = s.encode('utf-8', errors='replace').decode('cp1252', errors='replace')
                    if candidate and candidate != s:
                        return candidate
                except Exception:
                    pass
                # Try swapping bytes with latin1 as a last attempt
                try:
                    candidate = s.encode('utf-8', errors='replace').decode('latin1', errors='replace')
                    if candidate and candidate != s:
                        return candidate
                except Exception:
                    pass
                # Common replacements: replace lone '?' between letters with 'é','á','í','ó','ú','ã','õ','ç' heuristically
                # We'll try to guess by checking common Portuguese names with patterns. This is conservative.
                def _guess_replace(qs):
                    for rep in ['á','à','ã','â','é','ê','í','ó','ô','ú','ü','ç','Á','É','Í','Ó','Ú']:
                        cand = s.replace('?', rep)
                        # cheap heuristic: if result now contains only valid letters and spaces
                        if re.search(r'[\w\s\-\'\u00C0-\u017F]+$', cand):
                            return cand
                    return s
                try:
                    guessed = _guess_replace(s)
                    if guessed and guessed != s:
                        return guessed
                except Exception:
                    pass
            # nothing worked; return original
            return s

        # --- 5. Process the DataFrame ---
        scoreboard_list = []
        for _, row in df.iterrows():
            # Ensure 'Team' field is treated as string and repaired if needed
            team_field = row['Team'] if 'Team' in row and pd.notna(row['Team']) else ''
            team_name, user_name = parse_team_info(team_field)
            # also repair any remaining mojibake in parsed names
            team_name = _repair_text(team_name) if isinstance(team_name, str) else team_name
            user_name = _repair_text(user_name) if isinstance(user_name, str) else user_name
            competitor_data = {
                "rank": int(row['Rank']), "team_name": team_name,
                "user_name": user_name, "score": int(row['Score']),
                "penalty": int(row['Penalty']),
                "problems": {}
            }
            for key in problem_keys:
                if key in row:
                    competitor_data["problems"][key] = parse_problem_cell(row[key])
            scoreboard_list.append(competitor_data)
        
        # --- 6. Save the JSON file ---
        with open(output_filename, "w", encoding="utf-8") as f:
            json.dump(scoreboard_list, f, indent=2, ensure_ascii=False)
        
        print(f"🎉 Success! Data converted and saved to '{output_filename}'")

    except FileNotFoundError:
        print(f"❌ Error: The file '{input_filename}' was not found.")
        print("Please make sure the script and the Excel file are in the same directory.")
    except Exception as e:
        print(f"❌ An unexpected error occurred: {e}")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("----------------------------------------------------------------------")
        print("Uso incorreto!")
        print("Como usar: python nome_do_script.py <arquivo_de_entrada.xlsx> <arquivo_de_saida.json>")
        print("Exemplo:   python vJudgeParse.py \"scoreboard.xlsx\" \"scoreboard.json\"")
        print("----------------------------------------------------------------------")
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = sys.argv[2]
    
    convert_scoreboard_excel_to_json(input_file, output_file)
