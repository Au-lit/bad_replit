const swup = new Swup();
let g_is_light_theme = null;

//document.querySelector("#theme_changer").onclick(() => {
//    const root_elem = document.querySelector("#root-element");
//    if (g_is_light_theme === null) {
//        if (root_elem.style.filter === "none") { g_is_light_theme = false; }
//        else { g_is_light_theme = true; }
//    }
//    if (g_is_light_theme) { root_elem.style.filter = "invert(1.0)"; }
//    else { root_elem.style.filter = "none"; }
//    g_is_light_theme = !g_is_light_theme;
//});