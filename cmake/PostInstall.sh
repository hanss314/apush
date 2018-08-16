if type chsh >/dev/null 2>&1; then \
    echo To use apush as your login shell:; \
    grep -q -- "$1/apush" /etc/shells || echo -e " * add the line '$1/apush' to the file '/etc/shells':" \
    "\n   echo '$1/apush' | sudo tee -a /etc/shells > /dev/null"; \
    echo " * run 'chsh -s $1/apush'"; \
    echo; \
fi;
