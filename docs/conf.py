from cgitb import html

extensions = ["breathe"]

html_theme = "sphinx_rtd_theme"
html_title = "SolarSync"
html_logo = "ressource/solarsync_logo.png"

html_theme_options = {
    'logo_only': True,
    'style_nav_header_background': '#829F93', # Background color of the navigation bar
}

# Breathe configuration
breathe_default_project = "solarsync"