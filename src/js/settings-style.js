'use strict';

// Shared phone presentation only. Apps keep their own fields, drafts and save
// controllers. Export a string so bundled HTML and offline Clay use the same CSS.
module.exports = [
  ':root{color-scheme:light;--page:#f4f5f7;--card:#fff;--ink:#15181c;--muted:#5d6571;--line:#d9dee6;--field:#f4f5f7;--accent:#145ca3;--accent-ink:#fff;--error:#a1231d;--title-size:28px;--section-size:20px;--card-radius:12px;--control-height:44px;--save-height:48px;--focus-width:3px;--control-motion:144ms;--disclose-motion:160ms;--font-family:-apple-system,BlinkMacSystemFont,"Segoe UI",sans-serif}',
  '@media(prefers-color-scheme:dark){:root{color-scheme:dark;--page:#11151b;--card:#1d232d;--ink:#f3f5f7;--muted:#b5bfcc;--line:#465165;--field:#252e3b;--accent:#77bbff;--accent-ink:#10151b;--error:#ffb4ab}}',
  '*,*:before,*:after{box-sizing:border-box}',
  'html{background:var(--page);color:var(--ink);scroll-padding-bottom:calc(var(--footer-space,var(--actions-height,144px)) + 16px)}',
  'body{margin:0;background:var(--page);color:var(--ink);font:16px/1.5 var(--font-family)}',
  'main{max-width:512px;margin:auto;padding:24px 16px calc(var(--footer-space,var(--actions-height,144px)) + 24px)}',
  'header{margin-bottom:24px}header:after{content:"";display:block;width:48px;height:4px;border-radius:2px;background:var(--accent)}',
  'h1{margin:0;font-size:var(--title-size);line-height:1.2;letter-spacing:-.6px}header>p{margin:12px 0 16px;color:var(--muted)}',
  'h2{margin:0 0 16px;font-size:var(--section-size);line-height:1.3;font-weight:700;color:var(--ink)}h3{font-size:17px;margin:16px 0 8px}p{margin:0 0 16px}',
  '.card{margin:0 0 24px;padding:16px;background:var(--card);border:1px solid var(--line);border-radius:var(--card-radius)}.card>p:last-child{margin-bottom:0}',
  'button,input,select,textarea{font:inherit}',
  'button,select,input:not([type=checkbox]):not([type=hidden]),textarea{min-height:var(--control-height);border:1px solid var(--line);border-radius:8px;background:var(--field);color:var(--ink)}',
  'input,select,textarea{width:100%;min-width:0;padding:9px 12px}select{padding:9px 10px}',
  'button{padding:9px 12px;font-weight:650;line-height:1.5;text-align:center;cursor:pointer;overflow-wrap:anywhere}',
  'button:disabled{opacity:.45;cursor:default}select:disabled{opacity:.6}',
  'label{display:block;margin:16px 0 8px;font-weight:650}label input,label select,label textarea{display:block;margin-top:7px}',
  'input[type=checkbox]{width:22px;height:22px;padding:0;flex:0 0 22px;accent-color:var(--accent)}',
  'button:focus-visible,input:focus-visible,select:focus-visible,textarea:focus-visible,summary:focus-visible{outline:var(--focus-width) solid var(--accent);outline-offset:3px}',
  '.hint{font-size:14px;color:var(--muted);margin:8px 0 0;overflow-wrap:anywhere}',
  'details>summary{min-height:var(--control-height);display:list-item;line-height:24px;padding:10px 0;align-content:center;cursor:pointer;font-weight:650;color:var(--ink)}details[open]>summary{margin-bottom:16px}',
  'details>p{font-size:14px;color:var(--muted);margin-top:0}details>p:last-child{margin-bottom:0}',
  '.error,.status.error{color:var(--error)}',
  'footer{position:fixed;bottom:0;left:0;right:0;padding:12px 16px max(16px,env(safe-area-inset-bottom));background:var(--page);border-top:1px solid var(--line)}footer[data-flow=true]{position:static}',
  '.footer-inner{max-width:480px;margin:auto}.status{font-size:14px;line-height:1.5;color:var(--muted);margin:0 0 12px;min-height:21px;text-align:left}',
  '.actions{display:flex;gap:8px}.actions button{flex:1;min-width:0;overflow-wrap:anywhere}footer[data-flow=true] .actions{flex-direction:column}',
  '#save{min-height:var(--save-height);background:var(--accent);border-color:var(--accent);color:var(--accent-ink)}',
  '.sr-only{position:absolute;width:1px;height:1px;padding:0;margin:-1px;overflow:hidden;clip:rect(0,0,0,0);white-space:nowrap;border:0}[hidden]{display:none!important}',
  'a{color:var(--accent)}',
  'button,select,summary{transition:background-color var(--control-motion) ease-out,border-color var(--control-motion) ease-out,color var(--control-motion) ease-out}button:active:not(:disabled){transform:translateY(1px)}',
  'details[open]>:not(summary){animation:xoce-disclose var(--disclose-motion) ease-out}@keyframes xoce-disclose{from{opacity:0;transform:translateY(-3px)}to{opacity:1;transform:translateY(0)}}',
  '@media(max-width:340px){main{padding-left:12px;padding-right:12px}.card{padding-left:12px;padding-right:12px}footer{padding-left:12px;padding-right:12px}.actions{flex-wrap:wrap}.actions button{min-width:90px}}',
  '@media(max-width:260px){.actions{flex-direction:column}main{padding-top:16px}}',
  '@media(prefers-reduced-motion:reduce){*,*:before,*:after{transition:none!important;animation:none!important}button:active:not(:disabled){transform:none}}'
].join('\n');
