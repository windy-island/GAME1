
// 余計なテーブルを隠す
// クラスがついてなかったため、jsで無理やり隠した
// カスタムフッターを使って読み込んでいる。
titlesToHide = ["限定公開変数類", "非公開変数類"];

for (const element of document.querySelectorAll("h2")) {
	for (const title of titlesToHide) {
		if (element.textContent.indexOf(title) !== -1) {
			element.closest("table").style.display = "none";
			break;
		}
	}
}