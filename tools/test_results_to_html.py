import re
import json
from pathlib import Path
from typing import Dict, Any, List
import html

def load_test_results() -> Dict[str, Any]:
    results = {}
    results_path = Path("tests/results.json")
    
    if not results_path.exists():
        raise Exception(f"Test results file not found: {results_path}")
    
    with Path.open(results_path, "r") as f:
        results = json.load(f)
    
    if results:
        return results
    else:
        raise Exception("Can't load test results")

def extract_test_code(test_name: str) -> str:
    """Извлекает код теста из файлов .cppm"""
    test_files = list(Path("tests").glob("**/*.cppm"))
    
    for file_path in test_files:
        with Path.open(file_path, "r", encoding="utf-8") as f:
            content = f.read()
            # Ищем функцию теста
            pattern = rf"bool\s+{re.escape(test_name)}\s*\([^)]*\)\s*\{{(.*?)\}}"
            match = re.search(pattern, content, re.DOTALL)
            if match:
                code = match.group(1).strip()
                # Форматируем код
                return format_code(code, file_path.name)
    
    return "// Code not found"

def format_code(code: str, filename: str) -> str:
    """Форматирует код для отображения"""
    lines = code.split('\n')
    formatted = []
    
    for i, line in enumerate(lines, 1):
        # Подсветка ключевых слов C++
        line = html.escape(line)
        line = re.sub(r'\b(bool|return|if|else|for|while|try|catch|throw|auto|const|static|inline|virtual|override|final|public|private|protected|class|struct|enum|template|typename|namespace|using|import|export|module)\b', 
                     r'<span class="keyword">\1</span>', line)
        line = re.sub(r'\b(\\d+)\b', r'<span class="number">\1</span>', line)
        line = re.sub(r'("[^"]*")', r'<span class="string">\1</span>', line)
        
        formatted.append(f'<div class="code-line"><span class="line-number">{i:3d}</span> {line}</div>')
    
    return f'<div class="code-block" data-filename="{filename}">' + '\n'.join(formatted) + '</div>'

def generate_html(results: List[Dict[str, Any]]) -> str:
    """Генерирует HTML страницу"""
    
    # Статистика
    total_tests = sum(r.get("total", 0) for r in results)
    total_passed = sum(r.get("passed", 0) for r in results)
    total_failed = sum(r.get("failed", 0) for r in results)
    success_rate = (total_passed / total_tests * 100) if total_tests > 0 else 0
    
    # Генерируем HTML для каждого теста
    test_html = []
    for i, result in enumerate(results):
        test_name = result.get("test_name", "Unknown")
        test_desc = result.get("test_description", "")
        passed = result.get("passed", 0)
        failed = result.get("failed", 0)
        total = result.get("total", 0)
        success = result.get("success", False)
        
        # Статус бейдж
        status_class = "is-success" if success else "is-danger"
        status_text = "✓ Passed" if success else "✗ Failed"
        
        # Детали тестов
        details_html = []
        if "case_results" in result:
            for case in result["case_results"]:
                case_name = case.get("name", "")
                case_desc = case.get("description", "")
                case_passed = case.get("passed", False)
                case_error = case.get("error", "")
                
                case_status = "success" if case_passed else "danger"
                case_icon = "✓" if case_passed else "✗"
                
                # Код теста
                test_code = extract_test_code(case_name)
                
                details_html.append(f'''
                <div class="test-case">
                    <div class="test-case-header" onclick="toggleCode(this)">
                        <span class="test-case-status has-text-{case_status}">{case_icon}</span>
                        <span class="test-case-name">{case_name}</span>
                        <span class="test-case-description">{case_desc}</span>
                        <span class="toggle-icon">▼</span>
                    </div>
                    <div class="test-case-code" style="display: none;">
                        {test_code}
                        {f'<div class="error-message">Error: {html.escape(case_error)}</div>' if case_error else ''}
                    </div>
                </div>
                ''')
        
        test_html.append(f'''
        <div class="test-suite">
            <div class="test-suite-header">
                <div class="test-suite-title">
                    <h3>{test_name}</h3>
                    <span class="tag {status_class}">{status_text}</span>
                </div>
                <p class="test-suite-description">{test_desc}</p>
                <div class="test-stats">
                    <span class="stat"><span class="has-text-success">{passed}</span> passed</span>
                    <span class="stat"><span class="has-text-danger">{failed}</span> failed</span>
                    <span class="stat">{total} total</span>
                </div>
            </div>
            <div class="test-cases">
                {''.join(details_html)}
            </div>
        </div>
        ''')
    
    # Полный HTML
    return f'''<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Light Test Results</title>
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bulma@1.0.2/css/bulma.min.css">
    <style>
        :root {{
            --bg-dark: #1a1e2c;
            --bg-darker: #0f1322;
            --bg-card: #242a3a;
            --text-primary: #e5e9f0;
            --text-secondary: #a0a8b9;
            --border-color: #2e3440;
            --success-color: #5cb85c;
            --danger-color: #d9534f;
            --code-bg: #1e2434;
            --line-number-color: #4a5268;
            --keyword-color: #81a1c1;
            --string-color: #a3be8c;
            --number-color: #b48ead;
        }}
        
        body {{
            background-color: var(--bg-dark);
            color: var(--text-primary);
            font-family: 'JetBrains Mono', 'Fira Code', monospace;
            padding: 2rem;
            min-height: 100vh;
        }}
        
        .container {{
            max-width: 1200px;
            margin: 0 auto;
        }}
        
        h1 {{
            font-size: 2.5rem;
            font-weight: 600;
            margin-bottom: 1rem;
            color: var(--text-primary);
        }}
        
        h1 .light {{
            color: var(--text-secondary);
            font-weight: 300;
        }}
        
        .summary-card {{
            background: linear-gradient(135deg, var(--bg-card), var(--bg-darker));
            border-radius: 12px;
            padding: 2rem;
            margin-bottom: 2rem;
            box-shadow: 0 4px 20px rgba(0, 0, 0, 0.3);
            border: 1px solid var(--border-color);
        }}
        
        .summary-stats {{
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
            gap: 1.5rem;
            margin-top: 1.5rem;
        }}
        
        .stat-card {{
            text-align: center;
            padding: 1rem;
            background: rgba(0, 0, 0, 0.2);
            border-radius: 8px;
            border: 1px solid var(--border-color);
        }}
        
        .stat-value {{
            font-size: 2.5rem;
            font-weight: 700;
            line-height: 1.2;
        }}
        
        .stat-label {{
            color: var(--text-secondary);
            font-size: 0.9rem;
            text-transform: uppercase;
            letter-spacing: 1px;
        }}
        
        .progress {{
            height: 8px;
            background: var(--bg-darker);
            border-radius: 4px;
            overflow: hidden;
            margin: 1rem 0;
        }}
        
        .progress-bar {{
            height: 100%;
            background: linear-gradient(90deg, var(--success-color), #8cc084);
            border-radius: 4px;
            transition: width 0.3s ease;
        }}
        
        .test-suite {{
            background: var(--bg-card);
            border-radius: 10px;
            margin-bottom: 1.5rem;
            overflow: hidden;
            border: 1px solid var(--border-color);
            box-shadow: 0 2px 8px rgba(0, 0, 0, 0.2);
        }}
        
        .test-suite-header {{
            padding: 1.5rem;
            border-bottom: 1px solid var(--border-color);
            background: rgba(0, 0, 0, 0.2);
        }}
        
        .test-suite-title {{
            display: flex;
            align-items: center;
            justify-content: space-between;
            margin-bottom: 0.5rem;
        }}
        
        .test-suite-title h3 {{
            font-size: 1.3rem;
            font-weight: 600;
            color: var(--text-primary);
            margin: 0;
        }}
        
        .test-suite-description {{
            color: var(--text-secondary);
            margin: 0.5rem 0;
            font-size: 0.95rem;
        }}
        
        .test-stats {{
            display: flex;
            gap: 1rem;
            font-size: 0.9rem;
        }}
        
        .stat {{
            color: var(--text-secondary);
        }}
        
        .test-cases {{
            padding: 0.5rem;
        }}
        
        .test-case {{
            margin: 0.5rem;
            background: var(--bg-darker);
            border-radius: 6px;
            border: 1px solid var(--border-color);
        }}
        
        .test-case-header {{
            padding: 1rem;
            cursor: pointer;
            display: flex;
            align-items: center;
            gap: 1rem;
            transition: background 0.2s;
        }}
        
        .test-case-header:hover {{
            background: rgba(255, 255, 255, 0.05);
        }}
        
        .test-case-status {{
            font-weight: 700;
            min-width: 24px;
        }}
        
        .test-case-name {{
            font-weight: 600;
            color: var(--text-primary);
            min-width: 200px;
        }}
        
        .test-case-description {{
            color: var(--text-secondary);
            flex: 1;
            font-size: 0.9rem;
        }}
        
        .toggle-icon {{
            color: var(--text-secondary);
            font-size: 0.8rem;
            transition: transform 0.3s;
        }}
        
        .test-case-code {{
            padding: 1rem;
            border-top: 1px solid var(--border-color);
            background: var(--code-bg);
            font-family: 'JetBrains Mono', 'Fira Code', monospace;
            font-size: 0.85rem;
            line-height: 1.5;
            overflow-x: auto;
        }}
        
        .code-line {{
            display: flex;
            white-space: pre;
            color: #d8dee9;
        }}
        
        .line-number {{
            color: var(--line-number-color);
            padding-right: 1.5rem;
            text-align: right;
            user-select: none;
            min-width: 40px;
            border-right: 1px solid var(--border-color);
            margin-right: 1rem;
        }}
        
        .keyword {{ color: var(--keyword-color); font-weight: 600; }}
        .string {{ color: var(--string-color); }}
        .number {{ color: var(--number-color); }}
        
        .error-message {{
            margin-top: 1rem;
            padding: 1rem;
            background: rgba(217, 83, 79, 0.1);
            border: 1px solid var(--danger-color);
            border-radius: 4px;
            color: var(--danger-color);
            font-family: monospace;
        }}
        
        .tag {{
            font-size: 0.8rem;
            padding: 0.4rem 0.8rem;
            border-radius: 4px;
            font-weight: 600;
        }}
        
        .tag.is-success {{ background: var(--success-color); color: white; }}
        .tag.is-danger {{ background: var(--danger-color); color: white; }}
        
        .has-text-success {{ color: var(--success-color) !important; }}
        .has-text-danger {{ color: var(--danger-color) !important; }}
        
        .timestamp {{
            text-align: right;
            color: var(--text-secondary);
            font-size: 0.85rem;
            margin-top: 2rem;
            padding-top: 1rem;
            border-top: 1px solid var(--border-color);
        }}
        
        ::-webkit-scrollbar {{
            width: 10px;
            height: 10px;
        }}
        
        ::-webkit-scrollbar-track {{
            background: var(--bg-darker);
        }}
        
        ::-webkit-scrollbar-thumb {{
            background: var(--border-color);
            border-radius: 5px;
        }}
        
        ::-webkit-scrollbar-thumb:hover {{
            background: #4a5268;
        }}
    </style>
</head>
<body>
    <div class="container">
        <h1>light_test <span class="light">results</span></h1>
        
        <div class="summary-card">
            <h2 class="title is-4" style="color: var(--text-primary);">Summary</h2>
            <div class="progress">
                <div class="progress-bar" style="width: {success_rate:.1f}%;"></div>
            </div>
            <div class="summary-stats">
                <div class="stat-card">
                    <div class="stat-value">{total_tests}</div>
                    <div class="stat-label">Total Tests</div>
                </div>
                <div class="stat-card">
                    <div class="stat-value has-text-success">{total_passed}</div>
                    <div class="stat-label">Passed</div>
                </div>
                <div class="stat-card">
                    <div class="stat-value has-text-danger">{total_failed}</div>
                    <div class="stat-label">Failed</div>
                </div>
                <div class="stat-card">
                    <div class="stat-value">{success_rate:.1f}%</div>
                    <div class="stat-label">Success Rate</div>
                </div>
            </div>
        </div>
        
        <div class="test-results">
            {''.join(test_html)}
        </div>
        
        <div class="timestamp">
            Generated on {Path('tests/results.json').stat().st_mtime}
        </div>
    </div>
    
    <script>
        function toggleCode(header) {{
            const code = header.nextElementSibling;
            const icon = header.querySelector('.toggle-icon');
            
            if (code.style.display === 'none') {{
                code.style.display = 'block';
                icon.textContent = '▲';
            }} else {{
                code.style.display = 'none';
                icon.textContent = '▼';
            }}
        }}
        
        // Автоматически открывать упавшие тесты
        document.querySelectorAll('.test-case').forEach(case_ => {{
            const status = case_.querySelector('.test-case-status');
            if (status && status.classList.contains('has-text-danger')) {{
                const code = case_.querySelector('.test-case-code');
                const icon = case_.querySelector('.toggle-icon');
                if (code) {{
                    code.style.display = 'block';
                    if (icon) icon.textContent = '▲';
                }}
            }}
        }});
    </script>
</body>
</html>
'''

def main():
    # Загружаем результаты тестов
    results_data = load_test_results()
    
    # Преобразуем в список если это одиночный результат
    results = results_data if isinstance(results_data, list) else [results_data]
    
    # Генерируем HTML
    html_content = generate_html(results)
    
    # Сохраняем в файл
    output_path = Path("tests/results.html")
    with Path.open(output_path, "w", encoding="utf-8") as f:
        f.write(html_content)
    
    print(f"✅ Test results HTML generated: {output_path.absolute()}")
    print(f"📊 Summary: {sum(r.get('total', 0) for r in results) if isinstance(results, dict) else results} tests total")

if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        print(f"❌ Error: {str(e)}")