let isMenuOpen = false

function toggleMenu() {
  let button = document.querySelector('#btn-mobile')
  let menu = document.querySelector('#menu')

  if (isMenuOpen) {
    menu.classList.add('closeMenu')
    button.innerHTML = '<img id="abrirmenu" src="img/icones/menulistra.png" alt="">'
    isMenuOpen = false
    return
  }

  menu.classList.remove('closeMenu')
  button.innerHTML = '<img id="fecharmenu" src="img/icones/menufechar.png" alt="">'
  isMenuOpen = true
}