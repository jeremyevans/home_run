require File.expand_path('../../spec_helper', __FILE__)

describe "DateTime formatting methods" do
  it "#asctime and #ctime should use a format similar to Time" do
    DateTime.new(2008, 1, 2, 10, 20, 30).asctime.should == 'Wed Jan  2 10:20:30 2008'
    DateTime.new(2008, 1, 2, 10, 20, 30).ctime.should == 'Wed Jan  2 10:20:30 2008'
  end
  
  it "#to_s should use an ISO8601 format" do
    DateTime.new(2008, 1, 2, 10, 20, 30, 8/24.0).to_s.should == '2008-01-02T10:20:30+08:00'
  end

  it "#inspect should use an ISO8601 format" do
    DateTime.new(2008, 1, 2, 10, 20, 30, 8/24.0).inspect.should == '#<DateTime 2008-01-02T10:20:30+08:00>'
  end

  ruby_version_is "1.9" do
    it "#httpdate should use an HTTP format" do
      DateTime.new(2009, 1, 2, 3, 4, 5).httpdate.should ==  "Fri, 02 Jan 2009 03:04:05 GMT"
    end

    it "#iso8601 should use an ISO8601 format" do
      DateTime.new(2009, 1, 2, 3, 4, 5, 0.5).iso8601.should ==  "2009-01-02T03:04:05+12:00"
    end

    it "#jisx03010 should use an JIS X 0301 format" do
      DateTime.new(2009, 1, 2, 3, 4, 5, 0.5).jisx0301.should ==  "H21.01.02T03:04:05+12:00"
      DateTime.new(1988, 1, 2, 3, 4, 5, 0.5).jisx0301.should ==  "S63.01.02T03:04:05+12:00"
      DateTime.new(1925, 1, 2, 3, 4, 5, 0.5).jisx0301.should ==  "T14.01.02T03:04:05+12:00"
      DateTime.new(1911, 1, 2, 3, 4, 5, 0.5).jisx0301.should ==  "M44.01.02T03:04:05+12:00"
      DateTime.new(1873, 1, 2, 3, 4, 5, 0.5).jisx0301.should ==  "M06.01.02T03:04:05+12:00"
      DateTime.new(1872, 1, 2, 3, 4, 5, 0.5).jisx0301.should ==  "1872-01-02T03:04:05+12:00"
      DateTime.new(1867, 1, 2, 3, 4, 5, 0.5).jisx0301.should ==  "1867-01-02T03:04:05+12:00"
    end

    it "#rfc2822 should use an RFC2822 format" do
      DateTime.new(2009, 1, 2, 3, 4, 5, 0.5).rfc2822.should ==  "Fri, 2 Jan 2009 03:04:05 +1200"
    end

    it "#rfc822 should use an RFC822 format" do
      DateTime.new(2009, 1, 2, 3, 4, 5, 0.5).rfc822.should ==  "Fri, 2 Jan 2009 03:04:05 +1200"
    end

    it "#rfc3339 should use an RFC3339 format" do
      DateTime.new(2009, 1, 2, 3, 4, 5, 0.5).rfc3339.should ==  "2009-01-02T03:04:05+12:00"
    end

    it "#xmlschema should use an ISO8601 format" do
      DateTime.new(2009, 1, 2, 3, 4, 5, 0.5).xmlschema.should ==  "2009-01-02T03:04:05+12:00"
    end

    it "should handle fractional seconds if given an argument for iso8601, jisx0301, rfc3339, and xmlschema" do
      DateTime.new(2009, 1, 2, 3, 4, 5, 0.5).iso8601(4).should ==  "2009-01-02T03:04:05.0000+12:00"
      DateTime.new(2009, 1, 2, 3, 4, 5, 0.5).jisx0301(4).should ==  "H21.01.02T03:04:05.0000+12:00"
      DateTime.new(2009, 1, 2, 3, 4, 5, 0.5).rfc3339(4).should ==  "2009-01-02T03:04:05.0000+12:00"
      DateTime.new(2009, 1, 2, 3, 4, 5, 0.5).xmlschema(4).should ==  "2009-01-02T03:04:05.0000+12:00"
    end
  end
end
